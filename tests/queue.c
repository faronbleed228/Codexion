#include "codexion.h"
#include <assert.h>

int main(void)
{
    t_dongle d = ft_new_dongle(1);
    t_coder a = {0}, b = {0};
    t_simulation sim = {0};
    t_parsing pars = {0};
    a.cod_num = 1;
    b.cod_num = 2;
    pthread_mutex_init(&a.coder_mutex, NULL);
    pthread_mutex_init(&b.coder_mutex, NULL);
    fifo_queue(&d);
    assert(d.next_coder == -1);
    add_to_queue(&d, &a);
    edf_queue(&d);
    assert(d.queue_n == 1 && d.queue[0] == &a);
    add_to_queue(&d, &b);
    a.last_comp_time = 20;
    b.last_comp_time = 10;
    edf_queue(&d);
    assert(d.queue_n == 2 && d.next_coder == 2);
    assert(d.queue[0] == &b && d.queue[1] == &a);
    edf_queue(&d);
    assert(d.queue_n == 2 && d.queue[1] == &a);
    delete_coder(&d);
    assert(d.queue_n == 1 && d.queue[0] == &a);
    delete_coder(&d);
    a.last_comp_time = b.last_comp_time = 10;
    add_to_queue(&d, &a);
    add_to_queue(&d, &b);
    edf_queue(&d);
    assert(d.queue[0] == &a && d.queue[1] == &b);
    pthread_mutex_init(&sim.stop_lock, NULL);
    pthread_mutex_init(&d.dongle_lock, NULL);
    sim.stop_simulation = 1;
    sim.pars_struct = &pars;
    a.sim_struct = &sim;
    take_dongle(&a, &d);
    assert(!d.is_taken && d.owner == NULL && d.queue_n == 2);
    d.is_taken = true;
    d.owner = &b;
    leave_dongle(&a, &d);
    assert(d.is_taken && d.owner == &b);
    pthread_mutex_destroy(&d.dongle_lock);
    pthread_mutex_destroy(&sim.stop_lock);
    pthread_mutex_destroy(&a.coder_mutex);
    pthread_mutex_destroy(&b.coder_mutex);
    return 0;
}
