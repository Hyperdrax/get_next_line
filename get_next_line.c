/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: flhensel <flhensel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/06 14:58:42 by flhensel          #+#    #+#             */
/*   Updated: 2025/11/06 16:12:21 by flhensel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static char	*free_and_return(char *buffer, char *stash)
{
	free(buffer);
	free(stash);
	return (NULL);
}

static char	*handle_newline(char **stash_ptr, char *nl)
{
	char	*line;
	char	*tmp;

	line = ft_substr(*stash_ptr, 0, nl - *stash_ptr + 1);
	tmp = ft_strdup(nl + 1);
	free(*stash_ptr);
	*stash_ptr = tmp;
	if (!line || !tmp)
	{
		free(line);
		free(*stash_ptr);
		*stash_ptr = NULL;
		return (NULL);
	}
	if (tmp[0] == '\0')
	{
		free(*stash_ptr);
		*stash_ptr = NULL;
	}
	return (line);
}

static char	*extract_line(char **stash_ptr)
{
	char	*nl;
	char	*line;

	if (!*stash_ptr || (*stash_ptr)[0] == '\0')
	{
		if (*stash_ptr)
			free(*stash_ptr);
		*stash_ptr = NULL;
		return (NULL);
	}
	nl = ft_strchr(*stash_ptr, '\n');
	if (nl)
		return (handle_newline(stash_ptr, nl));
	line = ft_strdup(*stash_ptr);
	free(*stash_ptr);
	*stash_ptr = NULL;
	return (line);
}

static char	*read_line(int fd, char *stash)
{
	int		count;
	char	*buffer;
	char	*tmp;

	buffer = malloc(sizeof(char) * (BUFFER_SIZE + 1));
	if (!buffer)
		return (NULL);
	count = 1;
	while (count > 0 && (!stash || !ft_strchr(stash, '\n')))
	{
		count = read(fd, buffer, BUFFER_SIZE);
		if (count < 0)
			return (free_and_return(buffer, stash));
		if (count == 0)
			break ;
		buffer[count] = '\0';
		tmp = ft_strjoin(stash, buffer);
		free(stash);
		stash = tmp;
		if (!stash)
			return (free_and_return(buffer, stash));
	}
	free(buffer);
	return (stash);
}

char	*get_next_line(int fd)
{
	static char	*stash;
	char		*line;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	stash = read_line(fd, stash);
	line = extract_line(&stash);
	return (line);
}
