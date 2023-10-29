/**
 * @author: Raghu Sai Phani Sriraj Vemparala
 * @file  : mysort.cpp
 * @brief : This file contains the main function and parsing of parameters is performed.
 * @References: https://stackoverflow.com/questions/8793020/using-getopt-long-c-how-do-i-code-up-a-long-short-option-to-both-require-a
 *              https://linux.die.net/man/3/getopt_long
 *              https://www.geeksforgeeks.org/merge-sort/
 *              https://www.geeksforgeeks.org/quick-sort/
 *              
 * Date   : 21 september 2023
*/
#include <iostream>
#include <vector>
#include <getopt.h>
#include <fstream>
#include <string>
#include "counter.hpp"
using namespace std;

size_t thread_count = 2;
size_t num_iterations = 10;
int main(int argc,char* argv[]) 
{
    bool bar_flag = 0,lock_flag = 0;
    std::vector<int> v1;
    int c = 0, option_index =0;
    string my_name, output_file, barrier, lock;
    //structure for options to be parsed and validate
    static struct option long_options[] = {
            {"name",    no_argument, 0,  'h' },
            {"outputfile",  required_argument, 0,  'o' },
            {"total_threads",  optional_argument, 0,  't' },
            {"total_iterations",  optional_argument, 0,  'n' },
            {"bar",  required_argument, 0,  'b' },
            {"lock",  required_argument, 0,  'l' },
            {0,         0,                 0,  0 }
            };
    while (1) 
    {
        //passing arguments for further operation
        c = getopt_long(argc, argv, "ho:t:n:b:l:", long_options, &option_index);
        if (c == -1)
            break;
        //Switch case is used to store the arguments
       switch (c) 
       {
            case 'h':
                cout << "Raghu Sai Phani Sriraj Vemparala" << endl;
                return 0;
                break;
            case 'o':
                output_file  = std::string(optarg);
                break;
            case 't':
                thread_count = atoi(optarg);
                if(thread_count > 150)
                {
			        printf("ERROR; too many threads\n");
			        return 1;
                }
                //cout << "Thread count:" << thread_count<<endl;
                break;
            case 'n':
                num_iterations = atoi(optarg);
                break;
            case 'b':
                barrier = std::string(optarg);
                bar_flag = 1;
                cout <<barrier <<endl;
                break;
            case 'l':
                lock = std::string(optarg);
                lock_flag = 1;
                cout <<lock <<endl;
                break;
            default: 
                cout << "Invalid_argument" <<endl;
                break;
       }
    }
    //open output file or create a file
    ofstream outputfile(output_file);
    if(outputfile.is_open())
    {
        int final_count = 0;
        if(bar_flag == 1)
        {
            final_count = tas_func_init(thread_count,num_iterations,barrier,bar_flag);
            outputfile << final_count << endl;
        }
        else if(lock_flag == 1)
        {
            final_count = tas_func_init(thread_count,num_iterations,lock,bar_flag);
            outputfile << final_count << endl;
        }
        else
        {
            cout<<"Invalid_operation"<<endl;
        }   
    }
    else
    {
        cout << "Failed to open output file" << endl;
        return -1;
    }
    //close the output file
    outputfile.close();
    return 0;   
}