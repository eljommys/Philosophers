/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jserrano <jserrano@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/20 19:10:14 by jserrano          #+#    #+#             */
/*   Updated: 2021/03/03 21:04:05 by jserrano         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int		ft_strlen(const char *str)
{
	int i;

	i = 0;
	while (str && str[i])
		i++;
	return (i);
}

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

static	void	itoa_isnegative(long long int *n, int *negative)
{
	if (*n < 0)
	{
		*n *= -1;
		*negative = 1;
	}
}

char			*ft_itoa(long long int n)
{
	long long int		tmpn;
	int					len;
	int					negative;
	char				*str;

	tmpn = n;
	len = 2;
	negative = 0;
	itoa_isnegative(&n, &negative);
	while (tmpn /= 10)
		len++;
	len += negative;
	if ((str = (char*)malloc(sizeof(char) * len)) == NULL)
		return (NULL);
	str[--len] = '\0';
	while (len--)
	{
		str[len] = n % 10 + '0';
		n = n / 10;
	}
	if (negative)
		str[0] = '-';
	return (str);
}

typedef	struct	s_param t_param;

typedef	struct	s_philo
{
	char			*name;
	t_param			*param;
	char			eating;
	pthread_t		process;
	struct s_philo	*left;
	struct s_philo	*right;
}				t_philo;

typedef	struct	s_param
{
	int				n;
	int				die;
	int				eat;
	int				sleep;
	int				times;
	t_philo			*p;
	pthread_mutex_t	check;
	pthread_mutex_t	speak;
}				t_param;

void	init(t_param *param, int argc, char **argv)
{
	int i;

	param->n = ft_atoi(argv[1]);
	param->die = ft_atoi(argv[2]);
	param->eat = ft_atoi(argv[3]);
	param->sleep = ft_atoi(argv[4]);
	param->times = (argc == 6) ? ft_atoi(argv[5]) : -1;
	param->p = (t_philo *)malloc(sizeof(t_philo) * param->n);
	pthread_mutex_init(&(param->check), NULL);
	pthread_mutex_init(&(param->speak), NULL);
	i = -1;
	while (++i < param->n)
	{
		param->p[i].name = ft_itoa(i);
		param->p[i].param = param;
		param->p[i].eating = 0;
		param->p[i].left = (!i) ? &param->p[param->n - 1] : &param->p[i - 1];
		param->p[i].right = (i == param->n - 1) ? &param->p[0] : &param->p[i + 1];
	}
}

void	show(t_param *param)
{
	printf("n = %d\n", param->n);
	printf("die = %d\n", param->die);
	printf("eat = %d\n", param->eat);
	printf("sleep = %d\n", param->sleep);
	printf("times = %d\n\n", param->times);
}

void	*start(void *arg)
{
	t_philo *p;

	p = arg;
	while (1)
	{
		pthread_mutex_lock(&p->param->check);

			write(1, "\nleft = ", 8);
			write(1, p->left->name, ft_strlen(p->left->name));
			write(1, "\teating = ", 10);
			write(1, ft_itoa(p->left->eating), ft_strlen(ft_itoa(p->left->eating)));
			write(1, "\n", 1);

			write(1, "number = ", 9);
			write(1, p->name, ft_strlen(p->name));
			write(1, "\n", 1);

			write(1, "right = ", 8);
			write(1, p->right->name, ft_strlen(p->right->name));
			write(1, "\teating = ", 10);
			write(1, ft_itoa(p->right->eating), ft_strlen(ft_itoa(p->right->eating)));
			write(1, "\n\n", 2);

		if (!p->left->eating && !p->right->eating)
		{
			p->eating = 1;
			pthread_mutex_unlock(&p->param->check);
			pthread_mutex_lock(&p->param->speak);
			write(1, "\neaten\n\n", 8);
			usleep(100000);
			p->eating = 0;
			pthread_mutex_unlock(&p->param->speak);
			return NULL;
		}
		else
			pthread_mutex_unlock(&p->param->check);
	}
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
		if (pthread_create(&param.p[i].process, 0, start, (void *)&param.p[i]))
		{
			printf("1exit\n");
			return 1;
		}
	}
	i = -1;
	while (++i < param.n)
		pthread_join(param.p[i].process, NULL);
	
	return 0;
}
