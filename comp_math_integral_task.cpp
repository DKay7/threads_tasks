#include <math.h>
#include <stdio.h>
#include <pthread.h>

//flexxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

typedef struct thread_data {
   double lower_bound;
   double upper_bound;
   double step_size;
   double result;
} thread_data;

//flexxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

double func_to_integrate (double x);
double slow_solution(double lower_bound, double upper_bound, double stepsize);
void* fast_solution_one_thread (void* args);
double fast_solution (const double lower_bound, const double upper_bound, const double step_size, const int num_threads);

//flexxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

int main () {

    const int       num_threads   = 12;
    const double    lower_bound   = 0;
    const double    upper_bound   = 2;
    const double    step_size     = 1e-7;
    const double    sec_size      = (upper_bound - lower_bound) / (double) num_threads;
    double sum = 0;

    #ifndef FAST_SOLUTION
    sum  = slow_solution(lower_bound, upper_bound, step_size);
    #else
    sum = fast_solution(lower_bound, upper_bound, step_size, num_threads);
    #endif

    printf ("Int sqrt(4 - x^2) = %lf\n", sum);

    return 0;
}

//flexxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

double func_to_integrate (const double x) {
    return sqrt(4 - x*x);
}

double slow_solution(const double lower_bound, const double upper_bound, const double stepsize) {

    int section_lower_bound = 0;
    int section_upper_bound = 0;
    double sum = 0;

    for (double step = lower_bound; step <= upper_bound; step += stepsize){
        sum += func_to_integrate(step + stepsize / 2)  * stepsize;
    }

    return sum;
}

double fast_solution (const double lower_bound, const double upper_bound, const double step_size, const int num_threads) {
    
    pthread_t threads[num_threads];
    thread_data* threads_data = (thread_data*) calloc (num_threads, sizeof(*threads_data));

    double section_size = (upper_bound - lower_bound) / num_threads;
    double section_lower_bound = 0;
    double section_upper_bound = section_size;

    for (int thread_idx = 0; thread_idx < num_threads; ++thread_idx) {
        threads_data[thread_idx] = {
            .lower_bound = section_lower_bound,
            .upper_bound = section_upper_bound,
            .step_size = step_size
        };

        pthread_create(&threads[thread_idx], NULL, fast_solution_one_thread, &threads_data[thread_idx]);

        section_lower_bound = section_upper_bound;
        section_upper_bound += section_size; 
    }

    double sum = 0;
    for (int thread_idx = 0; thread_idx < num_threads; thread_idx++) {
        pthread_join(threads[thread_idx], NULL);
        sum += threads_data[thread_idx].result;
    }
    
    free (threads_data);

    return sum;
}

void* fast_solution_one_thread (void* args) {
    thread_data* data = (thread_data*) args;

    data->result = slow_solution (data->lower_bound, data->upper_bound, data->step_size);
    pthread_exit(NULL);
}