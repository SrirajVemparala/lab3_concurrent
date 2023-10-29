/**
 * @author: Raghu Sai Phani Sriraj Vemparala
 * @file  : bucket_sort.cpp
 * @brief : This file contains the information related to function definitions used for bucket_sorting algorithm.
 * Refernece: https://www.javatpoint.com/cpp-vector-insert-function
 *              
 * Date   : 21 september 2023
*/
#include "bucket_sort.hpp"
using namespace std;

extern size_t thread_count;

/**
 *This function provides length of the vector array
 * 
 */
int bs::vector_length()
{    
   return (int)v1.size();
}
/**
 *This function provides minimum element of the vector array
 * 
 */
int bs::min_element()
{
    std::vector<int>::iterator minElement = std::min_element(v1.begin(), v1.end());
    int min_ele = *minElement;
    return min_ele;

}

/**
 *This function provides maximum element of the vector array
 * 
 */
int bs::max_element()
{
    std::vector<int>::iterator maxElement = std::max_element(v1.begin(), v1.end());
    int max_ele = *maxElement;
    return max_ele;
}

/**
 *This function provides minimum length of the elements for each bucket
 * @param bucket_count total number of buckets
 * @param range the reange of each bucket
 * @param max_element maximum element of the vector array. 
 *
 */
void bs::find_min_elements(int bucket_count, int range, int max_element)
{
    for(int i = 0;i<= bucket_count;i++)
    {
        if(i != bucket_count)
        {
            mins.push_back((i)*range);
        }
        else
        {
            mins.push_back(max_element);// This ensures the maximum element is not missed from the mins values
        }
    }
}

/**
 *This function clears the vector array
 *
 */
void bs::clear_vector()
{
    v1.clear();
}
/**
 *This function writes data of each bucket into the vector array
 * @param buckets reference to the buckets
 * @param bucket_count the total number of buckets
 *
 *
 */
void bs::write_data(std::vector<vector<int>> &buckets, int bucket_count)
{
    for (int i = 0; i < (int)bucket_count; i++) 
    {
        v1.insert(v1.end(), buckets[i].begin(), buckets[i].end());//Pushing the sorted array into the vector
    }
}

/**
 *This function takes the input path of the input file and pushes data into the vector from the file
 * @param inputfile passes the input string to the file
 *
 */
void bs::pushdatatovector(const std::string& inputfile)
{
    ifstream inputFileStream(inputfile); // Open the file

    if (!inputFileStream.is_open()) {
        cout << "Failed to open input file: " << inputfile << endl;
        return;
    }

    string value;
    while (getline(inputFileStream, value))
    {
        int new_val = stoi(value);
        v1.push_back(new_val);
    }

    inputFileStream.close(); // Close the file when done
}

/**
 *This function takes the output file path of the output file and pushes data from the vector into the file
 * @param outputfile passes the output string to the file
 *
 */
void bs::outputtofile(const std::string& outputfile)
{
    ofstream outputFileStream(outputfile);
    for(int i : v1)
    {
        outputFileStream << i << endl;
    }
}

/**
 *This function takes the prints the vector of the file
 *
 */
void bs::print_vector()
{
    for(int i : v1)
    {
        printf("%d\n", i);
    }
}
