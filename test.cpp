/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yamajid <yamajid@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/09 22:07:59 by yamajid           #+#    #+#             */
/*   Updated: 2024/05/09 22:35:42 by yamajid          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <fstream>
#include <sstream>

#define  FILE_NAME "test.txt"

int main (){
    std::ifstream datafile(FILE);
    std::string line;

    while (std::getline(line, datafile)){
        std::cout << line << std::endl;
    }
}