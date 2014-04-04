#include <boost/program_options.hpp>
#include <boost/progress.hpp>
#include <boost/format.hpp>
#include <boost/timer.hpp>
#include "luash.h"

using namespace std;
using namespace lshkit;
namespace po = boost::program_options; 


typedef Tail<RepeatHash<ThresholdingLsh> > MyLsh;
typedef LshIndex<MyLsh, unsigned> Index;

Environment *init(int dim, int N, float *data_block){

    Environment *env = new Environment;

    env->dim = dim;
    env->index = (void *) (new Index());
    env->data = new FloatMatrix(dim,N,data_block);

    FloatMatrix data = *(env->data);
    Index index = *((Index *)(env->index));

    string index_file = "./test.index";

    float R =  numeric_limits<float>::max() ;
    unsigned M = 20, L = 1, H = 1017881;
    unsigned Q = 100, K = 50;
    bool do_benchmark = true;
    bool use_index = false; // load the index from a file

    
    // We define a short name for the MPLSH index.
    float min = numeric_limits<float>::max();
    float max = -numeric_limits<float>::max();

    for (unsigned i = 0; i < data.getSize(); ++i) {
        for (unsigned j = 0; j < data.getDim(); ++j) {
            if (data[i][j] > max) max = data[i][j];
            if (data[i][j] < min) min = data[i][j];
        }
    }

    Index::Parameter param;

    // Setup the parameters.  Note that L is not provided here.
    param.range = H;
    param.repeat = M;
    param.min = min;
    param.max = max;
    param.dim = data.getDim();
    DefaultRng rng;

    index.init(param, rng, L);

    cout << "BUILDING THE INDEX" << endl;
    for (unsigned i = 0; i < data.getSize(); ++i)
    {
        // Insert an item to the hash table.
        // Note that only the key is passed in here.
        // MPLSH will get the feature from the accessor.
        index.insert(i, data[i]);
    }
    cout << "DONE" << endl;

    return env;
}

int query(Environment *env, float *queryData){
    float R =  numeric_limits<float>::max() ;
    unsigned K = 50;
    metric::l1<float> l1(data.getDim());
    FloatMatrix::Accessor accessor(data);
    TopkScanner<FloatMatrix::Accessor, metric::l1<float> > query(accessor, l1, K, R);
    ((Index *)(env->index))->query(queryData, query);

    return query.topk()[1].key;
}



int run(void)
{
    string data_file = "/home/www/lsh/data/audio.data";
    string benchmark = "/home/www/lsh/data/audio.query";
    string index_file = "./test.index";

    float R =  numeric_limits<float>::max() ;
    unsigned M = 20, L = 1, H = 1017881;
    unsigned Q = 100, K = 50;
    bool do_benchmark = true;
    bool use_index = false; // load the index from a file

    boost::timer timer;

    cout << "LOADING DATA..." << endl;
    
    FloatMatrix data(data_file);
    cout << "DONE" << endl;


    metric::l1<float> l1(data.getDim());
    FloatMatrix::Accessor accessor(data);
    Index index;

    bool index_loaded = false;

    if (use_index) {
        ifstream is(index_file.c_str(), ios_base::binary);
        if (is) {
            is.exceptions(ios_base::eofbit | ios_base::failbit | ios_base::badbit);
            cout << "LOADING INDEX..." << endl;
            timer.restart();
            index.load(is);
            BOOST_VERIFY(is);
            cout << boost::format("LOAD TIME: %1%s.") % timer.elapsed() << endl;
            index_loaded = true;
        }
    }

    if (!index_loaded) {
        cout << "NO INDEX" << endl;
        // We define a short name for the MPLSH index.
        float min = numeric_limits<float>::max();
        float max = -numeric_limits<float>::max();

        for (unsigned i = 0; i < data.getSize(); ++i) {
            for (unsigned j = 0; j < data.getDim(); ++j) {
                if (data[i][j] > max) max = data[i][j];
                if (data[i][j] < min) min = data[i][j];
            }
        }

        Index::Parameter param;

        // Setup the parameters.  Note that L is not provided here.
        param.range = H;
        param.repeat = M;
        param.min = min;
        param.max = max;
        param.dim = data.getDim();
        DefaultRng rng;

        index.init(param, rng, L);
        // The accessor.

        // Initialize the index structure.  Note L is passed here.
        cout << "CONSTRUCTING INDEX..." << endl;

        timer.restart();
        {
            boost::progress_display progress(data.getSize());
            for (unsigned i = 0; i < data.getSize(); ++i)
            {
                // Insert an item to the hash table.
                // Note that only the key is passed in here.
                // MPLSH will get the feature from the accessor.
                index.insert(i, data[i]);
                ++progress;
            }
        }
        cout << boost::format("CONSTRUCTION TIME: %1%s.") % timer.elapsed() << endl;

        if (use_index) {
            timer.restart();
            cout << "SAVING INDEX..." << endl;
            {
                ofstream os(index_file.c_str(), ios_base::binary);
                os.exceptions(ios_base::eofbit | ios_base::failbit | ios_base::badbit);
                index.save(os);
                BOOST_VERIFY(os);
            }
            cout << boost::format("SAVING TIME: %1%s") % timer.elapsed() << endl;
        }
    }

    if (do_benchmark) {

        Benchmark<> bench;
        cout << "LOADING BENCHMARK..." << endl;
        bench.load(benchmark);
        bench.resize(Q, K);
        cout << "DONE." << endl;

        for (unsigned i = 0; i < Q; ++i)
        {
            for (unsigned j = 0; j < K; ++j)
            {
                assert(bench.getAnswer(i)[j].key < data.getSize());
            }
        }

        cout << "RUNNING QUERIES..." << endl;

        Stat recall;
        Stat cost;

        timer.restart();
        {
            TopkScanner<FloatMatrix::Accessor, metric::l1<float> > query(accessor, l1, K, R);
            boost::progress_display progress(Q);
            for (unsigned i = 0; i < Q; ++i)
            {
                query.reset(data[bench.getQuery(i)]);
                index.query(data[bench.getQuery(i)], query);
                recall << bench.getAnswer(i).recall(query.topk());
                cost << double(query.cnt())/double(data.getSize());
                ++progress;
            }
        }
        cout << boost::format("QUERY TIME: %1%s.") % timer.elapsed() << endl;

        cout << "[RECALL] " << recall.getAvg() << " +/- " << recall.getStd() << endl;
        cout << "[COST] " << cost.getAvg() << " +/- " << cost.getStd() << endl;

    }

    return 0;
}

