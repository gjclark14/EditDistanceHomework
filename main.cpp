#include <iostream>
#include <vector>
#include <tuple>
#include <algorithm>
#include <iomanip>

//#define DEBUG


class Calculator {
private:
    std::vector<std::vector<int>> matrix;
    std::vector<std::tuple<int, int>> coordinateVector;
    std::string inputString;
    std::string targetString;

public:
    Calculator(std::string inputString, std::string targetString) {
        this->inputString = targetString;
        this->targetString = inputString;
        matrix = generateMatrix(this->inputString,this->targetString);
        coordinateVector = generateCoordinateVector(matrix);

        printMatrix();
        printf("\n");
        printAlignment(coordinateVector, this->inputString, this->targetString);
    }


    int min(const int & a, const int & b, const int & c) {
        int min = a;
        if (b < min)
            min = b;
        if (c < min)
            min = c;
        return min;
    }

// Returns min value
// and index of min value
    std::tuple<int, int> getMinIndices(const std::vector<std::vector<int>> &matrix, const int & minimum, const int &i,
                                       const int &j) {
        // substitution
        if (minimum == matrix[i - 1][j - 1]) {
            return {i - 1, j - 1};

            // horizontal =
            // note this was matrix[i+1][j] before
        } else if (minimum == matrix[i-1][j]) {
            return {i - 1, j};

        } else {
            return {i, j - 1};
        }
    }

// This is assuming row column, ensures that I am using row column before any of this starts
// to avoid a lot of frustration.
// basically just create a coordinate vector
// with this coordinate vector check what the distance between v_i and v_i+1 is, i < vector length - 1
// if the distance between v_i and v_i+1 is (1,1) we know that we moved diagonally ()
// if the distance between v_i and v_i+1 is (1,0) we know that we moved horizontally (

    int getX(const std::tuple<int,int> & pair) {
        return std::get<0>(pair);
    }

    int getY(const std::tuple<int,int> & pair) {
        return std::get<1>(pair);
    }

    void printAlignment(std::vector<std::tuple<int, int>> coVec, std::string str1, std::string str2) {

        const int LEN = coVec.size();
#ifdef DEBUG
        std::printf("\n\nLEN = %d\n\n", LEN);
#endif

        for(int i = 0; i < LEN - 1; i++) {
            int xDistance = abs(getX(coVec.at(i)) - getX(coVec.at(i+1)));
            int yDistance = abs(getY(coVec.at(i)) - getY(coVec.at(i+1)));

#ifdef DEBUG
            std::printf("i = %d\n",i);
        std::printf("The distance between the coordinate vector[i] = (%d,%d)\n\tand vector[i+1] = (%d,%d) is,\n\t\t(%d,%d)\n\n" ,
                getX(coVec.at(i)), getY(coVec.at(i)),
                getX(coVec.at(i+1)), getY(coVec.at(i+1)),
                xDistance, yDistance);
#endif

            if(xDistance == 1 and yDistance == 1) {
                // we moved diagonal = substitution

            } else if (xDistance == 1 and yDistance == 0) {
                // we moved horizontally = deletion
                str1.replace(i,0, "_");

            } else {
                // we moved vertically = insertion
                str2.insert(i, "_");
            }

        }

        int mSize = matrix.size() - 1;
        int mSize2 = matrix.at(0).size() - 1;
        std::printf("The edit distance is: %d\n\n", matrix[mSize][mSize2]);

        std::printf("Alignment is:\n");
        std::printf("%s\n", str2.c_str());
        std::printf("%s\n", str1.c_str());

    }


// find the minimum in the surrounding three vertices. move to that
    std::vector<std::tuple<int, int>> generateCoordinateVector(const std::vector<std::vector<int>> & matrix) {

        const int COLS = matrix.size();
        const int ROWS = matrix.at(0).size();

        int minimum = 0;

        int i = COLS - 1, j = ROWS - 1;

        std::vector<std::tuple<int, int>> coVec;

#ifdef DEBUG
        printf("Coordinates: (%d,%d)\n",i,j);
#endif

        coVec.push_back(std::tuple<int,int>(i,j));
        while(i > 0 and j > 0) {
            minimum = min(matrix[i - 1][j],matrix[i][j - 1],matrix[i - 1][j - 1]);




            std::tuple<int,int> indices = getMinIndices(matrix, minimum, i, j);
            coVec.push_back(indices);
            i = std::get<0>(indices), j = std::get<1>(indices);

#ifdef DEBUG
            printf("Coordinates: (%d,%d)\n",i,j);
#endif

            if(i ==  0 and j > 0)  {
                do {
                    j--;
                    coVec.push_back(std::tuple<int,int>(i,j));
                } while (j > 0);
            } else if (i > 0 and j == 0) {
                do {
                    i--;
                    coVec.push_back(std::tuple<int,int>(i,j));
                } while (i > 0);
            }
        }

        std::reverse(std::begin(coVec), std::end(coVec));
        return coVec;
    }



    void printMatrix() {
        std::printf("The matrix:\n\n");

        const int ROWS = matrix.size();
        const int COLS = matrix.at(0).size();
        std::cout << std::left << std::setw(4) << " ";
        for(int i = 0; i < COLS; i++)
            std::cout << std::setw(3) << std::right << i;
        printf("\n\n");

        for(int i = 0; i < ROWS; i++) {
            std::cout << std::left << std::setw(4) << i;
            for(int j = 0; j < COLS; j++) {
                std::cout << std::setw(3) << std::right << matrix[i][j];
            }
            printf("\n");
        }
    }

    std::vector<std::vector<int>> generateMatrix(const std::string & str1, const std::string & str2) {
        // matrix strings
        std::string mString1 = "-" + str1;
        const int ROWS = mString1.size();

        std::string mString2 = "-" + str2;
        const int COLS = mString2.size();

        // Empty matrix initialized to all zeros
        std::vector<std::vector<int>> matrix (COLS,std::vector<int>(ROWS, 0));

        // Init row 0 and col 0
        for(int i = 0; i < ROWS; i++)
            matrix[0][i] = i;

        for(int i = 0; i < COLS; i++)
            matrix[i][0] = i;

        for(int i = 1; i < COLS; i++) {
            for(int j = 1; j < ROWS; j++) {
                const int SUB_COST = (mString1.at(j) == mString2.at(i)) ? 0 : 1;
                matrix[i][j] = min(matrix[i-1][j] + 1,            // deletion
                                   matrix[i][j-1] + 1,            // insertion
                                   matrix[i-1][j-1] + SUB_COST);  // substitution
            }
        }


        return matrix;
    }


};


int main() {
    std::string str1 = "Revolution";

    std::string str2 = "Evaluation";

    if(str1.length() > 30 or str2.length() > 30) {
        std::printf("String length over 30 error.\n");
        return -1;
    }

    Calculator c(str1,str2);



    return 0;
}