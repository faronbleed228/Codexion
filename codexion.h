#ifndef CODEXION_H
# define CODEXION_H

# include <pthread.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/time.h>
# include <unistd.h>

typedef struct s_dongle
{
	pthread_mutex_t	lock;
	pthread_cond_t	cond;
	int				dongle_id;
	long long		last_comp_t;
	bool			is_taken;
}					t_dongle;

typedef enum e_status
{
	COMPILING = 1,
	DEBUGGING = 2,
	REFACTORING = 3,
	WAITING = 4

}					status;

typedef struct s_coder
{
	int				cod_num;
	long long		last_comp_time;
	int				n_compiles;
	t_dongle		*left_dongle;
	t_dongle		*right_dongle;
	status			cur_status;
	pthread_t		thread_id;

}					t_coder;

typedef struct s_parsing
{
	int				coders;
	long long		t_to_burn;
	long long		t_to_copm;
	long long		t_to_debug;
	long long		t_to_refac;
	int				copm_req;
	long long		cooldown;
	char			*scheduler;
	long long		start_time;
}					t_parsing;

typedef struct s_simulation
{
	pthread_mutex_t	lock;
	int				stop_simulation;
	t_coder			*coder_array;
	t_dongle		*dongle_array;
	t_parsing		*pars_struct;
	long long		start_time;
}					t_simulation;

int					simulation_start(t_simulation *sim_struct);

t_parsing			*parsing(char **argv);
t_simulation		*simulation_init(t_parsing *pars_struct);

int					ft_isdigit(int c);
long long			get_cur_time_ns(struct timeval tv);

t_coder				ft_new_coder(int coder_id);
t_dongle			ft_new_dongle(int dongle_id);

void				free_sim(t_simulation *sim_struct);
int					free_mutex(t_simulation *sim_struct, int counter);
int					free_cond(t_simulation *sim_struct, int counter);
void				ft_end_thread(int counter, t_simulation *sim_struct);
#endif
