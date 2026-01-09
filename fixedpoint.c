//compile: # gcc fixedpoint.c -lm

//arithmetic:
//addition:       augend     + addend       = sum
//subtraction:    minuend    − subtrahend   = difference
//multiplication: multiplier × multiplicand = product
//division:       dividend   ÷ divisor      = quotient and remainder

//we do arithmetic operation on mathematical numbers,
//(positive, negative, fractional etc.) using machines.
//everything in machine is stored as 1 and 0
//value of a bit pattern stored in a byte
//depends on number representation scheme in use.
//10000000 is interpreted as 128, sometimes as -128,
//(128 for unsigned number representation scheme,
//and -128 for signed 2's complement number representation scheme)
//even, if one wishes, can interpret it as something else!
//lets interpret it as something else! lets proceed...

//here our interest is to perform arithmetic operations on numbers
//having fractional part, but without using floating point instructions.
//we can do so using fixed point number representation scheme,
//and using only integer instructions, which is faster!
//fixed point numbers are so named because in such a number representation,
//binary point is imagined to be present at a fixed position between binary digits.
//we don't explicitly store exponent in the number representation
//as we do in floating point number representation scheme.
//floating point number representation gives flexibility of
//moving binary point and thus more precision.
//fixed point number representation offers speed for arithmetic calculations,
//but with lesser precision!

//lets run away from conventional number representation schemes and
//define our own number representation scheme!
//lets say we imagined that, we will represent our numbers using 8 bits,
//and we will imagine having a binary point in the middle of 3rd and 4th digit from right.
//now we will see previous bit pattern 10000000 as 10000.000
//and immediately the value of this bit pattern becomes -16.000!
//remember, value of a bit pattern depends on our interpretation,
//i.e. according to our number representation scheme.
//so, using our scheme, we now have numbers from 00000.000 to 01111.111 and 10000.000 to 11111.111.
//whose value is from 0.000 to 15.875 and from -16.000 to -0.125 respectively.
//lets say this is our 5.3 digit scheme for number representation.
//5 bits for integral part, 3 bits for fractional part!

//lets say we imagined that, we will represent our numbers using 8 bits,
//but we will imagine the place of binary point at right side of 1st digit!
//then we have values of 8 bit variables ranging from 0 to 127 and from -128 to -1.
//which is our usual interpretation of 8 bit variables.
//lets say this is our 8.0 digit scheme for number representation.
//8 bits for integral part, 0 bits for fractional part!

//if we wish, we can also imagine to place the binary point in the middle of 2nd and 3rd digit from right.
//then we can have numbers from 000000.00 to 011111.11 and 100000.00 to 111111.11.
//which is from 0.00 to 31.75 and from -32.00 to -0.25.
//lets say this is our 6.2 digit scheme for number representation.
//6 bits for integral part, 2 bits for fractional part!

//note that place of our binary point determines the precision of our numbers.
//also notice that we can't represent some numbers like 1.815 or 1.82
//using our 5.3 or 6.2 digit scheme!
//if there are 'q' digits after binary point, granularity of the scheme is 2^(-q)

//it is very important to note that C integer arithmetic
//with 8 bit variables assume 8.0 digit scheme.
//so when we want to perform arithmetic operation on numbers
//represented in our own number representation schemes,
//we have to appropriately fill C variables,
//only with numerical values allowable from 8.0 digit scheme,
//do the arithmetic operations, and then
//interpret value of resultant C variable as per our number representation scheme.

//what we really or effectively do is, just for a while,
//we forget our original number representation scheme,
//we move to another number representation scheme
//fill variables with values as per new scheme
//and we do all operations as needed, being in new scheme
//and interpret the result back in our original scheme!

//example: say we want to add/sub/mul/div numbers of 5.3 digit scheme.
//first we have to get bit pattern of our number in our scheme.
//we have to assign the same bit patterns to C variables.
//better to say, we have to assign appropriate numerical value
//to C variables as per 8.0 digit scheme.
//this effectively means, for a while we changed our scheme to 8.0 digit scheme!
//now perform operations on those variables using integer instructions.
//interpret the result back in our 5.3 scheme. Done!
//similarly when we will do arithmetic on 6.2 digit numbers,
//we will change to 8.0 digit scheme and be back.

#include<stdio.h>
#include<math.h>

//this function interpretes the value of 8 bit C variable
//in our own number representation scheme.

//it takes total digits to be used for our number representation scheme,
//digits for integral part (digits for fractional part can then be derived),
//and some value in a 8 bit container.

//from the given scheme and bits in the container,
//this function calculates interpreted value of 8 bit container in our scheme.

//notice that even though our container is 8 bit wide,
//our scheme need not be 8 bit wide!
//we can have dgt_tot = 4 and dgt_int_part = 3!

//     <--p digits----> <--q digits-->
//    |_|_|_|_|_|_|_|_|.|_|_|_|_|_|_|_|
//    p-1          1 0  -1 -2       -q
//
//value = (-1)^b[p-1]*b[p-1]*2^(p-1) + ... + b[1]*2^(1) + b[0]*2^(0) + b[-1]*2^(-1) + ... + b[-q]*2^(-q)
double value(int dgt_tot, int dgt_int_part, unsigned char b)
{
        double val=0;
        int j;

        printf("%d = ", (signed char)b);
        for (j=dgt_tot-1; j>=0; j--) //for each digit, msb to lsb
        {
                int d; //each digit
                double term;

                d = (b>>j)&1;
                printf("%d", d);

                term = d*pow(2, dgt_int_part-1);
                dgt_int_part--;

                if(j==dgt_tot-1 && d==1){ term = -term; }
                //printf(":%f\n", term);

                val += term;
        }
        printf(" = %f\n", val);
        return val;
}

int main()
{
        int i;
        unsigned char a, b, r;

        //print each 5.3 digit binary number
        for (i=0; i<=255; i++)
        {
                value(8, 5, i);
        }
        printf("\n");

        //our need is to perform 0.375 + 2.125
        //0.375 in 5.3 digit scheme will be 00000.011
        //to put this bit pattern into C variable, we have to change to 8.0 digit scheme
        //in 8.0 digit scheme above bit pattern evaluates to 3 (forget about binary point!)
        //recall that binary point is our imagination in our scheme!
        //so we filled 'a' with 3 (for 0.375)
        a=3; //0.375
        value(8, 5, a); //lets recheck if bit pattern in 'a' evaluates to 0.375 in our scheme!

        //similarly we filled 'b' with 17 (for 2.125)
        b=17; //2.125
        value(8, 5, b);

        //lets do a+b; note that addition is performed in 8.0 digit scheme
        r = a + b;
        //now we have to interpret result 'r' using our 5.3 digit scheme!
        value(8, 5, r); //2.5
        //and we found that interpreted result is same as actual math answer!

        //similarly lets do subtractions and check answers
        r = b - a;
        value(8, 5, r); //1.75
        r = a - b;
        value(8, 5, r); //-1.75
        printf("\n");

        //some more examples
        value(8, 5, 63); //7.875
        value(8, 5, 64); //8.0
        value(8, 5, 63+64); //15.875
        value(8, 5, 64-63); //0.125
        value(8, 5, 63-64); //-0.125
        printf("\n");

        value(8, 5, 92); //11.5
        value(8, 5, 34); //4.25
        value(8, 5, 92+34); //15.75, note that addition result should be bounded by 127
        value(8, 5, 34-92); //-7.25, note that subtraction result should be bounded by -128
        value(8, 5, 92-34); //7.25
        printf("\n");

        //print each 6.2 digit binary number
        for (i=0; i<=255; i++)
        {
                value(8, 6, i);
        }
        printf("\n");


        //multiplication: 1.75 * 2.00
        //answer: 3.50
        //for multiplication, we first ignore decimal point
        //so 175 * 200 = 35000
        //then we put decimal point leaving 4 digits from right
        //so answer is 3.5000

        //in our 6.2 digit scheme 1.75 is 01.11
        //0111 evaluates to 7 in 8.0 digit scheme
        //similarly 2.00 is 10.00 and 1000 is 8
        //now we can perform 0111 * 1000
        //which is 7 * 8 = 56 = 111000

        //time to place binary point! it is imaginary! so what to do?
        //can we interpret this result in our 6.2 digit scheme to get the answer?
        //no; if we do so, we will interpret result as 1110.00 which is wrong!
        //experience the 'fixed' part of fixed point number schemes!
        //we should interpret the result as 11.1000
        //since we have to put binary point leaving 4 digits from right

        //how to do this? we can't do this!
        //only way is to right shift result by 2 and
        //only then our interpretation using 6.2 digit scheme
        //will interpret as 11.10 whis is correct.
        //(sometimes such shift may result in precision loss!)
        //so answer is 11.10 which is nothing but 3.50 in our scheme
        value(8, 6, 7); //1.75
        value(8, 6, 8); //2.0
        value(8, 6, (7*8)>>2); //3.5
        printf("\n");

        value(8, 6, 12); //3.0
        value(8, 6, 8); //2.0
        value(8, 6, (12*8)>>2); //6.0
        printf("\n");

        value(8, 6, 7); //1.75
        value(8, 6, 9); //2.25
        value(8, 6, (7*9)>>2); //3.75 ~ 3.9375(actual answer), notice the precision loss!
        printf("\n");

        //division: 4.75 / 3.25
        //answer: 1.46
        //in our 6.2 digit scheme 4.75 is 100.11
        //10011 evaluates to 19 in 8.0 digit scheme
        //similarly 3.25 is 11.01 which is 13

        //similar to multiplication, division is done ignoring decimal point,
        //and then we have to appropriately adjust decimal point in result
        //here we have 4.75 / 3.25 = 475 / 325 = 1
        //(for the time being think of it as integer division only)
        //since both dividend and divisor have same number of digits after decimal point
        //no adjustment of decimal point is needed on the result

        //like wise we have 19 / 13 = 1
        //recall that we thought of using integer division instructions only
        //which will never give us fractional part!
        //so how should we proceed to interpret the result?
        //unfortunately our interpretation of 6.2 digit scheme
        //will attempt to interpret the result
        //as if it has two digit fractional part, which is wrong!

        //experience again the 'fixed' part of fixed point number schemes!
        //since result has no fractional part, we should interpret the result
        //as if whole result constitutes the integral part of our scheme
        //which will be 1.00 and the correct answer.
        //so to put result into our integral part, result must be shifted to left by 2.
        value(8, 6, 19); //4.75
        value(8, 6, 13); //3.25
        value(8, 6, (19/13)<<2); //1 ~ 1.46(actual answer), notice the precision loss!
        printf("\n");

        //but we lost precision a lot due to nature of integer division
        //can we do anything about it? yes, we can.
        //to get little more precision, we can multiply dividend by some factor
        //and later divide result by the same factor!
        //which is nothing but suitable adjustment of decimal/binary point!
        value(8, 6, 19*2); //4.75 * 2
        value(8, 6, 13); //3.25
        value(8, 6, ((19*2)/13)<<1); //1 ~ 1.46(actual answer), notice the precision loss!
        //why we didn't do left shift of the result by 2 as before?
        //here though the result is a integer, no fractional part,
        //we can't completely push the result to integral part of our scheme
        //if we do so, our result will be answer of (4.75*2)/3.25, not of 4.75/3.25!
        //we have to divide result by 2 since we had multiplied dividend by 2 before
        //which is nothing but placing binary point after one digit from left
        //our interpretation of 6.2 digit scheme implicitly places binary point after two digit from left
        //so correct result is achieved by left shift by 1.
        printf("\n");

        //but we didn't get any better precision by multiplying dividend by 2!
        //to get little more precision, we can multiply dividend by 4
        //and later divide result by 4!
        //which is nothing but suitable adjustment of decimal/binary point again!
        value(8, 6, 19*4); //4.75 * 4
        value(8, 6, 13); //3.25
        value(8, 6, ((19*4)/13)); //1.25 ~ 1.46(actual answer), notice the precision loss!
        //why we didn't do left shift of the result by 1 as before?
        //we have to divide result by 4 since we had multiplied dividend by 4 before
        //which is nothing but placing binary point after two digit from left
        //fortunately our interpretation of 6.2 digit scheme implicitly places binary point after two digit from left
        //so correct result is achieved by no shift at all!
        printf("\n");

        //in all arithmetic operations happening in 8.0 digit scheme,
        //we have to very carefully such that no overflow ever occurs!
        //that's the case at which one needs to move to 16 bit or 32 bit variables.

        return 0;
}

