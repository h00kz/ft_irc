/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffeaugas <ffeaugas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/19 14:01:26 by ffeaugas          #+#    #+#             */
/*   Updated: 2023/04/19 17:58:16 by ffeaugas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"

int getNbWords(std::string str)
{
    int nbWords;
    std::stringstream ss(str);

    nbWords = 0;
    while (ss >> str)
        nbWords++;
    return (nbWords);
}