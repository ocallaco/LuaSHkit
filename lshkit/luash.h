#include <boost/program_options.hpp>
#include <boost/progress.hpp>
#include <boost/format.hpp>
#include <boost/timer.hpp>
#include <lshkit.h>

typedef Tail<RepeatHash<ThresholdingLsh> > MyLsh;
typedef LshIndex<MyLsh, unsigned> Index;

typedef struct {
    int dim;
    Index *index;
    FloatMatrix *data;    
} Environment;


extern "C" {
    Environment *init(int dim, int N, float *data_block);
    int query(Environment *env, float *queryData);
}
