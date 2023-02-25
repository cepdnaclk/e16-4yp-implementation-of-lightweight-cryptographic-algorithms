#include <cstdint>
#include <vector>
#include <iostream>
//#include "distribution.hpp"
#include <cstdio>

/*
 * The probability of each bit of bytes being 1. 
 * Starting from the MSB
 */
const double s0_bit_prob[8] = {0.25, 0.25, 0.25, 0.25, 0.25, 0.25, 0.25, 0.25}; 
const double s47_bit_prob[8] = {0.25, 0.25, 0.25, 0.25, 0.25, 0.25, 0.25, 0.5}; 
const double s70_bit_prob[8] = {0.25, 0.25, 0.25, 0.25, 0.25, 0.25, 0.5, 0.5}; 
const double s85_bit_prob[8] = {0.25, 0.25, 0.25, 0.25, 0.25, 0.5, 0.5, 0.5}; 
const double s91_bit_prob[8] = {0.25, 0.25, 0.25, 0.25, 0.25, 0.25, 0.25, 0.25}; 


double find_bit_prob(int bit);


std::vector<float> calculate_s_faulty_distribution(const std::vector<double>& bit_probs)
{
    std::vector<float> dest(256, 1.0);
    for (int i = 0; i < 256; i++)
    {
	uint8_t byte = i;
	for (int j = 0; j < 8; j++)
	    dest[i] *= (byte >> (7 - j)) & 0x01 ? bit_probs[j] : 1.0 - bit_probs[j];
    }

    return dest;	
}

std::vector<double> calculate_bit_probs()
{
    std::vector<double> bit_probs(8);
    for (int i = 0; i < 8; i++)
	bit_probs[i] = find_bit_prob(i);
    return bit_probs;
}
	
double find_bit_prob(int bit)
{
    double prob_1 = 0.0;
//    for (uint8_t i = 0; i < 2; i++)
//	for (uint8_t j = 0; j < 2; j++)
//	    for (uint8_t k = 0; k < 2; k++)
//		for (uint8_t m = 0; m < 2; m++)
//		    for (uint8_t n = 0; n < 2; n++)
//		    {
//			double s0_prob = i == 1 ? s0_bit_prob[bit] : 1.0 - s0_bit_prob[bit];
//			double s47_prob = j == 1 ? s47_bit_prob[bit] : 1.0 - s47_bit_prob[bit];
//			double s70_prob = k == 1 ? s70_bit_prob[bit] : 1.0 - s70_bit_prob[bit];
//			double s85_prob = m == 1 ? s85_bit_prob[bit] : 1.0 - s85_bit_prob[bit];
//			double s91_prob = n == 1 ? s91_bit_prob[bit] : 1.0 - s91_bit_prob[bit];
//			if (((i ^ j ^ (~(k & m)) ^ n) & 0x01) == 1)
//			    prob_1 += s0_prob * s47_prob * s70_prob * s85_prob * s91_prob;
//		    }
    for (uint8_t s0 = 0; s0 < 2; s0++)
	for (uint8_t s47 = 0; s47 < 2; s47++)
	    for (uint8_t s70 = 0; s70 < 2; s70++)
		for (uint8_t s85 = 0; s85 < 2; s85++)
	    {
	        double s0_prob = s0 == 1 ? s0_bit_prob[bit] : 1.0 - s0_bit_prob[bit];
	        double s47_prob = s47 == 1 ? s47_bit_prob[bit] : 1.0 - s47_bit_prob[bit];
	        double s70_prob = s70 == 1 ? s70_bit_prob[bit] : 1.0 - s70_bit_prob[bit];
	        double s85_prob = s85 == 1 ? s85_bit_prob[bit] : 1.0 - s85_bit_prob[bit];
		if (((s0 ^ s47 ^ (~(s70 & s85)) ) & 0x01) == 0x01)
		    prob_1 += s0_prob * s47_prob * s70_prob * s85_prob;
	    }

    return prob_1;
}

int main()
{
    auto distribution = calculate_s_faulty_distribution(calculate_bit_probs());
    double sum = 0;
    std::cout << "size: " << distribution.size() << std::endl;
    for (int i = 0; i < 32; i++)
    {
	for (int j = 0; j < 8; j++)
	{
	    printf("%.8f, ", distribution[i * 8 + j]);
	    sum += distribution[i * 8 + j];
	}
	printf("\n");
    }
    std::cout << std::endl;
    std::cout << "sum: " << sum << std::endl;
}
