/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_map.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mneves-l <mneves-l@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/21 16:19:22 by gsilva            #+#    #+#             */
/*   Updated: 2024/05/12 14:42:20 by mneves-l         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cub3d.h"

int		fill_map(char *file);
char	*fill_line(int fd);
int		is_valid_char(int c);
int		parse_map(void);

int	fill_map(char *file)
{
	int		i;
	int		fd;
	char	*line;

	i = 0;
	if (fill_el(file, -1) == -1 || check_img() == -1)
		return (-1);
	fd = open(file, O_RDONLY);
	if (fd < 0)
		error("Opening the file", 0);
	while (i++ <= map()->last_elem)
	{
		line = get_next_line(fd);
		free(line);
	}
	map()->map = (char **)malloc(sizeof(char *) * (map()->lines + 2));
	i = -1;
	while (++i <= map()->lines)
		map()->map[i] = fill_line(fd);
	map()->map[i] = NULL;
	close(fd);
	map()->p = 0;
	return (parse_map());
}

char	*fill_line(int fd)
{
	char	*line;
	char	*get_line;
	int		l;
	int		i;

	get_line = get_next_line(fd);
	if (!get_line)
		return (NULL);
	line = (char *)calloc(map()->max_len + 1, sizeof(char));
	i = 0;
	l = map()->max_len;
	while (--l > ft_strlen_nl(get_line))
		line[i++] = ' ';
	while (--l >= 0)
		line[i++] = get_line[l];
	line[i] = '\0';
	free(get_line);
	return (line);
}

int	is_valid_char(int c)
{
	if (c != '1' && c != '0' && c != 'W' && c != 'E'
		&& c != 'N' && c != 'S' && !ft_isspace(c))
		return (0);
	if (c == 'W' || c == 'E' || c == 'N' || c == 'S')
	{
		if (map()->p != 0)
			return (0);
		map()->p = c;
	}
	return (1);
}

int	parse_map(void)
{
	int	i;
	int	j;

	i = -1;
	while (map()->map[++i])
	{
		j = -1;
		while (map()->map[i][++j])
		{
			if (map()->p == 0)
			{
				plr()->mapy = i;
				plr()->mapx = j;
			}
			if (!is_valid_char(map()->map[i][j]))
				return (error("Invalid character in map", 0));
		}
	}
	if (map()->p == 0)
		return (error("Missing player", 0));
	if (check_rgb(map()->elements[4], 'F') || check_rgb(map()->elements[5], 0))
		return (error("Problem with rgb", 0));
	return (parse_path());
}
