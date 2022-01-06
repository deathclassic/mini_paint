#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <libc.h>
#include <math.h>

typedef struct s_map{
	int		width;
	int		height;
	char	b;
}	t_map;

typedef struct s_config{
	char	c;
	float	x;
	float	y;
	float	r;
	char	fill;
}	t_config;

typedef struct s_pic{
	char			*line;
	struct s_pic	*next;
}	t_pic;

typedef struct s_list{
	t_config		circ;
	struct s_list	*next;
}	t_list;

t_pic	*init_picture(char *str)
{
	t_pic *pic;

	pic = (t_pic *)malloc(sizeof(t_pic));
	pic->line = str;
	pic->next = NULL;
	return (pic);
}

void	add_pic(t_pic **pic, char *str)
{
	t_pic	*start;

	if(!*pic)
	{
		*pic = init_picture(str);
		return ;
	}
	start = *pic;
	while((*pic)->next)
		*pic = (*pic)->next;
	(*pic)->next = init_picture(str);
	*pic = start;
}

t_list	*init_list(t_config tmp)
{
	t_list *lst;

	lst = (t_list *)malloc(sizeof(t_list));
	lst->circ.c = tmp.c;
	lst->circ.x = tmp.x;
	lst->circ.y = tmp.y;
	lst->circ.r = tmp.r;
	lst->circ.fill = tmp.fill;
	lst->next = NULL;
	return (lst);
}

void	add_list(t_list **lst, t_config tmp)
{
	t_list	*start;

	start = NULL;
	if (!*lst)
	{
		*lst = init_list(tmp);
		return ;
	}
	start = *lst;
	while((*lst)->next)
		*lst = (*lst)->next;
	(*lst)->next = init_list(tmp);
	*lst = start;
}

void	print_list(t_list *lst)
{
	while (lst)
	{
		printf("data: %c %f %f %f %c\n", lst->circ.c, lst->circ.x, lst->circ.y, lst->circ.r, lst->circ.fill);
		lst = lst->next;
	}
}

void	error_exit(int i)
{
	if (i == 0)
	{
		printf("Error: argument\n");
		exit (1);
	}
	if (i == 1)
	{
		printf ("Error: Operation file corrupted\n");
		exit (1);
	}
}

void	print_pic(char **pic)
{
	int	i = 0;

	while(pic[i])
	{
		printf("%s\n", pic[i]);
		i++;
	}
}

char	**pic_to_char(t_pic *pic)
{
	char	**str;
	t_pic	*start;
	int		len = 0;
	int		i = 0;

	start = pic;
	while(pic)
	{
		pic = pic->next;
		len++;
	}
	pic = start;
	str = (char **)malloc(sizeof(char *) * (len + 1));
	while(pic)
	{
		str[i] = pic->line;
		i++;
		pic = pic->next;
	}
	str[i] = NULL;
	return (str);
}

void	paint_circle(t_list *lst, char **pic, t_map map)
{
	float	dist;
	int		i;
	int		j;

	while (lst)
	{
		i = 0;
		j = 0;
		if (lst->circ.c == 'c')
		{
			while (i < map.height)
			{
				while (j < map.width)
				{
					dist = sqrtf((j - lst->circ.x)*(j - lst->circ.x) + (i - lst->circ.y)*(i - lst->circ.y));
					if (dist <= lst->circ.r && dist > lst->circ.r - 1)
						pic[j][i] = lst->circ.fill;
					j++;
					
				}
				i++;
				j = 0;
			}
		}
		else if (lst->circ.c == 'C')
		{
			while (i < map.height)
			{
				while (j < map.width)
				{
					dist = sqrtf((j - lst->circ.x)*(j - lst->circ.x) + (i - lst->circ.y)*(i - lst->circ.y));
					if (dist <= lst->circ.r)
						pic[i][j] = lst->circ.fill;
					j++;
				}
				i++;
				j = 0;
			}
		}
		lst = lst->next;
	}
	print_pic(pic);
}

void	mini_paint(t_map map, t_list *lst)
{
	t_pic	*pic;
	char	**ch_pic;
	char	*tmp;
	int		i;
	int		j;

	j = 0;
	pic = NULL;
	while (j < map.height)
	{
		i = 0;
		tmp = malloc(map.width + 1);
		while(i < map.width)
		{
			tmp[i] =  map.b;
			i++;
		}
		tmp[i] = 0;
		add_pic(&pic, tmp);
		free(tmp);
		j++;
	}
	ch_pic = pic_to_char(pic);
	paint_circle(lst, ch_pic, map);
}

int main(int argc, char **argv)
{
	t_config	tmp;
	FILE 		*fp;
	t_list		*lst;
	t_map		map;

	if (argc != 2)
		error_exit(0);
	if (!(fp = fopen(argv[1], "r+")))
		error_exit(1);
	fscanf(fp, "%d %d %c\n", &map.width, &map.height, &map.b);
	if ((map.width > 300 || map.width <= 0) || (map.height <= 0 || map.height > 300))
		error_exit(1);
	while (fscanf(fp, "%c %f %f %f %c\n", &tmp.c, &tmp.x, &tmp.y, &tmp.r, &tmp.fill) > 0)
	{
		if (tmp.r <= 0 || (tmp.c != 'C' && tmp.c != 'c'))
			error_exit(1);
		add_list(&lst, tmp);
	}
	mini_paint(map, lst);
	fclose(fp);
	return (0);
}