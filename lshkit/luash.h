#include <lshkit.h>

extern "C" {
    Index *init(void);
    int run(void);
    void insert(Index *index, int i, float *data);
    int query(Index *index, float *query);
}
