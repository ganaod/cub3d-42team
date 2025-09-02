/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclear_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blohrer <blohrer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 17:19:18 by blohrer           #+#    #+#             */
/*   Updated: 2025/02/28 09:22:55 by blohrer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstclear(t_list **lst, void (*del)(void *))
{
	t_list	*temp;

	if (!lst || !*lst)
		return ;
	while (*lst)
	{
		temp = (*lst)->next;
		printf("DEBUG: Lösche Node %p\n", (void *)*lst);
		if ((*lst)->content && ((uintptr_t)(*lst)->content > 0x1000))
		{
			printf("DEBUG: Lösche Content %p\n", (void *)(*lst)->content);
			del((*lst)->content);
		}
		free(*lst);
		*lst = temp;
	}
}
