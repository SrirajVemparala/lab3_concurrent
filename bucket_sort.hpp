/**
 * @author: Raghu Sai Phani Sriraj Vemparala
 * @file  : bucket_sort.hpp
 * @brief : This file contains the information related to variables and functions used for bucket_sorting algorithm.
 * 
 *              
 * Date   : 21 september 2023
*/
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>
using namespace std;
class bs
{
    public:
        std::vector<int> v1;
        std::vector<int> mins;
        int vector_length();
        int min_element();
        int max_element();
        void find_min_elements(int, int, int);
        void clear_vector();
        void write_data(std::vector<vector<int>>& , int);
        void outputtofile(const std::string& );
        void print_vector();
        void pushdatatovector(const std::string& );

};