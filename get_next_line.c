#include "get_next_line.h"

static char *free_and_return(char *buffer, char *stash)
{
    free(buffer);
    free(stash);
    return (NULL);
}

static char *handle_newline(char **stash_ptr, char *nl)
{
    char *line;
    char *tmp;
    
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
    return (line);
}

static char *extract_line(char **stash_ptr)
{
    char *nl;
    char *line;
    
    if (!*stash_ptr)
        return (NULL);
    nl = ft_strchr(*stash_ptr, '\n');
    if (nl)
        return (handle_newline(stash_ptr, nl));
    line = ft_strdup(*stash_ptr);
    free(*stash_ptr);
    *stash_ptr = NULL;
    return (line);
}

static char *join_buffer(char *stash, char *buffer)
{
    char *tmp;
    
    tmp = ft_strjoin(stash, buffer);
    if (!tmp)
        return (NULL);
    free(stash);
    return (tmp);
}

static char	*read_line(int fd, char *stash)
{
	int		count;
	char	*buffer;

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
		stash = join_buffer(stash, buffer);
		if (!stash)
			return (free_and_return(buffer, NULL));
	}
	free(buffer);
	return (stash);
}

char *get_next_line(int fd)
{
    static char *stash;
    char *line;
    
    if (fd < 0 || BUFFER_SIZE <= 0)
        return (NULL);
    stash = read_line(fd, stash);
    if (!stash)
        return (NULL);
    line = extract_line(&stash);
    return (line);
}