/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zserobia <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 14:40:48 by zserobia          #+#    #+#             */
/*   Updated: 2024/10/29 14:40:51 by zserobia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_list(t_token **list)
{
	t_token *current;
	t_token *next_node;

	current = *list;
	while (current) {
		next_node = current->next;
		free(current->value);
		free(current);
		current = next_node;
	}
	*list = NULL;
}

t_token	*token_create(char *value, int type, int index)
{
	t_token	*new;

	new = (t_token *)malloc(sizeof(t_token));
	if (!new)
		return (NULL);
	new->type = type;
	new->value = (value != NULL) ? strdup(value) : strdup("NULL");
	new->next = NULL;
	//new->prev = NULL;
	new->index = index;
	return (new);
}

void	token_add_back(t_token **list, t_token *new_token)
{
	t_token	*new;

	if (*list)
	{
		new = *list;
		while (new->next != NULL)
			new = new->next;
		new->next = new_token;
		//new_token->prev = new;
	}
	else
	{
		*list = new_token;
	}
}

int	ft_ifspace(char *line)
{
	if ((*line >= 9 && *line <= 13) || *line == ' ')
			return (1);
	return (0);
}

void	ft_create_list(char *value, int type, int index, t_token **list)
{
	t_token *new_token;

	new_token = token_create(value, type, index);
	if (new_token)
		token_add_back(list, new_token);
}

int	ft_read_word(char *line)
{
	int	i;

	i = 0;
	while (line[i] && !ft_ifspace(&line[i]) && line[i] != '|' && line[i] != '>' && line[i] != '<') {
		i++;
	}
	return (i);
}

int	ft_read_word_1(char *line)
{
	int	i;

	i = 1;
	while (line[i] && line[i] != '\'')
	{
		i++;
	}
	return (i);
}

char *substr(const char *str, size_t start, size_t len)
{
	size_t str_len = strlen(str);
	
	// Проверяем, что начальный индекс не выходит за границу строки
	if (start >= str_len) return NULL;
	// Корректируем длину подстроки, если она выходит за конец строки
	if (start + len > str_len)
	{
		len = str_len - start;
	}
	// Выделяем память для подстроки + 1 для нуль-терминатора
	char *sub = malloc(len + 1);
	if (!sub) return NULL;
	// Копируем подстроку и добавляем нуль-терминатор
	strncpy(sub, str + start, len);
	sub[len] = '\0';
	return sub;
}
t_token *ft_read_line(char *line, t_token **list)
{
	int	index;

	index = 0;
	while(*line)
	{
		while (ft_ifspace(line))
			line++;
		if (*line == '|')
			ft_create_list(NULL, T_PIPE, index, list);
		else if (*line == '>' && *(line + 1) == '>')
		{
			ft_create_list(NULL, T_PLUS2, index, list);
			line++;
		}
		else if (*line == '>' && *(line + 1) != '>')
			ft_create_list(NULL, T_PLUS, index, list);
		else if (*line == '<' && *(line + 1) == '<')
		{
			ft_create_list(NULL, T_LESS2, index, list);
			line++;
		}
		else if (*line == '<' && *(line + 1) != '<')
			ft_create_list(NULL, T_LESS, index, list);
		else if (*line == '/'')
		{
			int c = ft_read_word_1(line, '/'');
			char *word = substr(line, 0, c);
			ft_create_list(word, T_WORD, index, list);
			line += c - 1;
		}
		else if
		{
			int c = ft_read_word(line);
			char *word = substr(line, 0, c);
			ft_create_list(word, T_WORD, index, list);
			line += c - 1;
		}
		line++;
		index++;
	}
	return (*list);
}

int	main()
{
	char *line;
	t_token *list = NULL;
	/*line = readline("Enter input: ");
	if (!line) {
		perror("Error reading line");
		return 1;
	}*/
	line = "Hello world | you are >>the best < merci";
	ft_read_line(line, &list);
	t_token *current = list; // Временный указатель для итерации
	while (current) {
		printf("Value - %s, type - %d, index - %d\n", current->value, current->type, current->index);
		current = current->next; // Переход к следующему токену
	}
	free_list(&list);
	return 0;
}