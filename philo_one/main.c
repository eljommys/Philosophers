/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/20 19:10:14 by jserrano          #+#    #+#             */
/*   Updated: 2021/02/28 15:55:49 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int		ft_atoi(const char *str)
{
	long long int	var[3];

	var[0] = 0;
	var[1] = 1;
	var[2] = 0;
	while (str[var[2]] == 32 || (9 <= str[var[2]] && str[var[2]] <= 13))
		var[2]++;
	if (str[var[2]] == '-' || str[var[2]] == '+')
		var[1] = (str[var[2]++] == '-') ? -1 : 1;
	while (str[var[2]])
	{
		if (str[var[2]] < 48 || 57 < str[var[2]])
			return (var[0] * var[1]);
		else
			var[0] = (var[0] * 10) + (long long int)(str[var[2]] - '0');
		if (var[0] < -2147483648)
			return (1);
		else if (2147483648 < var[0])
			return (0);
		var[2]++;
	}
	return ((long int)(var[0] * var[1]));
}

typedef	struct	s_param
{
	int				n;
	int				die;
	int				eat;
	int				sleep;
	int				times;
	pthread_t		*p;
	pthread_mutex_t	lock;
}				t_param;

void	init(t_param *param, int argc, char **argv)
{
	param->n = ft_atoi(argv[1]);
	param->die = ft_atoi(argv[2]);
	param->eat = ft_atoi(argv[3]);
	param->sleep = ft_atoi(argv[4]);
	param->times = (argc == 6) ? ft_atoi(argv[5]) : -1;
	param->p = (pthread_t *)malloc(sizeof(pthread_t) * param->n);
	pthread_mutex_init(&param->lock, NULL);
}

void	show(t_param param)
{
	printf("n = %d\n", param.n);
	printf("die = %d\n", param.die);
	printf("eat = %d\n", param.eat);
	printf("sleep = %d\n", param.sleep);
	printf("times = %d\n\n", param.times);
}

void	*start(void *arg)
{
	t_param *param;

	param = arg;
	pthread_mutex_lock(&param->lock);
	show(*param);
	pthread_mutex_unlock(&param->lock);
	return NULL;
}

int		main(int argc, char **argv)
{
	t_param	param;
	int		process;
	int		i;

	if (argc != 5 && argc != 6)
	{
		printf("0exit\n");
		return 1;
	}
	init(&param, argc, argv);
	i = -1;
	while (++i < param.n)
	{
		if (pthread_create(&param.p[i], 0, start, (void *)&param))
		{
			printf("1exit\n");
			return 1;
		}
		//pthread_join(param.p[i], NULL);
	}
	printf("2exit\n");
	pthread_mutex_destroy(&param.lock);
	return 0;
}
