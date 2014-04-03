#include <lshkit.h>

extern "C" {
    void *init(void);
    int run(void);
    void insert(void *index, int i, float *data);
    int query(void *index, float *query);
}
