/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ywang2 <ywang2@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/17 15:41:54 by ywang2            #+#    #+#             */
/*   Updated: 2025/11/25 13:48:15 by ywang2           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H

# include <unistd.h>
# include <stdarg.h>
# include <stdlib.h>
# include "ft_printf.h"

int	ft_print_char(char c);
int	ft_print_str(char *s);
int	ft_print_nbr(long nb);
int	ft_print_hex(unsigned int nb, char format);

int	ft_put_ptr(unsigned long long ads);
int	ft_print_ptr(unsigned long long ads);
int	ft_print_format(va_list args, const char format);
int	ft_printf(const char *str, ...);

#endif