/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncotte <marvin@42lausanne.ch>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/20 10:48:08 by ncotte            #+#    #+#             */
/*   Updated: 2022/12/20 10:48:09 by ncotte           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/// \brief Check if str contains a valid long int (one char at the time)
/// \param str the string to convert (source)
/// \param nbr the converted number (so far)
/// \param index the position in str of the current char to check
/// \return 1 if it does, 0 otherwise
static int	is_numeric(char const *str, long nbr, int index)
{
	int	i;
	int	last_digit;

	i = 0;
	last_digit = (int)(LONG_MAX % 10) + 1;
	if (str[0] == '-' || str[0] == '+')
		i++;
	return (!(index - i > 18 || (index - i == 18 && nbr >= LONG_MAX / 10 \
		&& ((str[0] != '-' && str[index] - '0' >= last_digit) \
		|| (str[0] == '-' && str[index] - '0' > last_digit)))));
}

/// \brief Convert a string into a long int
/// \param str the string to convert (source)
/// \param nbr a pointer to the number (destination)
/// \return 1 on SUCCESS, 0 if ERROR
static int	ft_atol(char *str, long *nbr)
{
	long	sign;
	int		i;

	i = 0;
	sign = 1L;
	*nbr = 0;
	if (!str)
		return (NO);
	if (str[i] == '+' || str[i] == '-')
		if (str[i++] == '-')
			sign *= -1L;
	while (ft_isdigit(str[i]) && is_numeric(str, *nbr, i))
		*nbr = *nbr * 10 + (str[i++] - '0');
	if (str[i])
		return (NO);
	*nbr *= sign;
	return (YES);
}

int	ft_exit(char *argv[])
{
	long	arg_value;
	int		ret_value;

	ret_value = ft_atol(argv[1], &arg_value);
	if (ret_value && argv[2])
		return (print_custom_error("minishell: exit: too many arguments"));
	g_var.exit = YES;
	ft_putendl_fd("exit", STDOUT_FILENO);
	if (argv[1] && !ret_value)
		return (print_error("minishell: exit", argv[1], \
			"numeric argument required", UNDEFINED_ERROR));
	return ((int)(arg_value & 0x00000000000000FF));
}
