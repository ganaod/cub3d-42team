/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blohrer <blohrer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 11:02:31 by blohrer           #+#    #+#             */
/*   Updated: 2025/09/22 10:28:43 by blohrer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

size_t	gnl_strlen(const char *str)
{
	size_t	len;

	len = 0;
	while (str && str[len] != '\0')
		len++;
	return (len);
}

char	*gnl_strchr(const char *str, int c)
{
	if (!str)
		return (NULL);
	while (*str)
	{
		if (*str == (char)c)
			return ((char *)str);
		str++;
	}
	if ((char)c == '\0')
		return ((char *)str);
	return (NULL);
}

char	*gnl_strdup(const char *s1)
{
	size_t	len;
	char	*copy;
	size_t	i;

	len = gnl_strlen(s1);
	copy = (char *)malloc(sizeof(char) * (len + 1));
	if (!copy)
		return (NULL);
	i = 0;
	while (s1[i])
	{
		copy[i] = s1[i];
		i++;
	}
	copy[i] = '\0';
	return (copy);
}

static void	copy_to_result(char *dest, const char *src, size_t start)
{
	size_t	i;

	i = 0;
	while (src && src[i])
	{
		dest[start + i] = src[i];
		i++;
	}
}

char	*gnl_strjoin(char const *s1, char const *s2)
{
	size_t	len1;
	size_t	len2;
	char	*result;

	len1 = 0;
	len2 = 0;
	if (s1)
		len1 = gnl_strlen(s1);
	if (s2)
		len2 = gnl_strlen(s2);
	result = (char *)malloc(sizeof(char) * (len1 + len2 + 1));
	if (!result)
	{
		if (s1)
			free((char *)s1);
		return (NULL);
	}
	copy_to_result(result, s1, 0);
	copy_to_result(result, s2, len1);
	result[len1 + len2] = '\0';
	if (s1)
		free((char *)s1);
	return (result);
}
