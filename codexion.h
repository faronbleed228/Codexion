#ifndef CODEXION_H
# define CODEXION_H

# include <pthread.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/time.h>
# include <unistd.h>

typedef struct s_simulation	t_simulation;
typedef struct s_coder		t_coder;

typedef struct s_queue
{
	t_coder					**queue;
	int						head;
	int						capacity;
	int						tail;
	int						count;
	pthread_mutex_t			queue_lock;
}							t_queue;

typedef struct s_free
{
	int						coder_thread;
	int						sim_thread;
	int						coder_mutex;
	int						sim_mutex;
	int						dongle_mutex;
	int						cond;
	int						queue_mutex;
}							t_free;

typedef struct s_dongle
{
	pthread_mutex_t			dongle_lock;
	pthread_cond_t			cond;
	int						dongle_id;
	long long				last_used_t;
	bool					is_taken;
}							t_dongle;

typedef enum e_status
{
	COMPILING = 0,
	DEBUGGING = 1,
	REFACTORING = 2,
	TOOK_DONGLE = 3,
	BURNED_OUT = 4,
	WAITING = 5,

}							status;

typedef struct s_coder
{
	pthread_mutex_t			coder_mutex;
	int						cod_num;
	long long				last_comp_time;
	int						n_compiles;
	t_dongle				*left_dongle;
	t_dongle				*right_dongle;
	status					cur_status;
	pthread_t				thread_id;
	t_simulation			*sim_struct;

}							t_coder;

typedef struct s_parsing
{
	int						coders;
	long long				t_to_burn;
	long long				t_to_copm;
	long long				t_to_debug;
	long long				t_to_refac;
	int						copm_req;
	long long				cooldown;
	char					*scheduler;
	long long				start_time;
}							t_parsing;

typedef struct s_simulation
{
	pthread_t				thread_id;
	pthread_mutex_t			stop_lock;
	pthread_mutex_t			log_lock;
	int						stop_simulation;
	t_coder					*coder_array;
	t_dongle				*dongle_array;
	t_parsing				*pars_struct;
	t_free					*free_struct;
	t_queue					*queue_struct;
	long long				start_time;
}							t_simulation;

int							simulation_start(t_simulation *sim_struct);
void						choose_dongle(t_coder *cur_coder);
void						take_dongle(t_coder *coder,
								t_dongle *dongle_struct);
void						leave_dongle(t_dongle *dongle_struct);
void						coder_compiling(t_coder *coder);
void						*monitor(void *void_struct);
int							check_compiles(t_simulation *sim_struct);
void						*stop_sim(t_simulation *sim_struct, t_coder *coder);
t_coder						*check_burnout(t_simulation *sim_struct);
bool						check_simulation(t_simulation *sim_struct);

t_parsing					*parsing(char **argv);
t_simulation				*simulation_init(t_parsing *pars_struct);

int							ft_isdigit(int c);
long long					get_cur_time_ms(struct timeval tv);
struct timespec				get_absolute_time(long long waiting);
void						log_output(t_coder *coder, status message);

t_coder						ft_new_coder(int coder_id,
								t_simulation *sim_struct);
t_dongle					ft_new_dongle(int dongle_id,
								t_simulation *sim_struct);

void						free_sim(t_simulation *sim_struct);
void						free_everything(t_simulation *sim_struct,
								t_free *free_struct);
void						join_threads(t_simulation *sim_struct,
								t_free *free_struct);

t_queue						*queue_init(t_simulation *sim_struct);
#endif
