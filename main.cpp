#include <iostream>
#include <vector>
#include <tuple>

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
    if (minimum == matrix[i - 1][j - 1]) {
        return {i - 1, j - 1};
    } else if (minimum == matrix[i+1][j]) {
        return {i - 1, j};
    } else {
        return {i, j - 1};
    }
}


// find the minimum in the surrounding three vertices. move to that
void printAlignment(const std::vector<std::vector<int>> & matrix) {

    const int COLS = matrix.size();
    const int ROWS = matrix.at(0).size();

    int minimum = 0;

    int i = COLS - 1, j = ROWS - 1;

    // This is skipping the bottom-right-most element because we are searching for the previous ones
    printf("Coordinates: (%d,%d)\n",j,i);
    while(i > 0 and j > 0) {
        minimum = min(matrix[i - 1][j],     // deletion
                      matrix[i][j - 1],     // insertion
                      matrix[i - 1][j - 1]);  // substitution

        std::tuple<int,int> indices = getMinIndices(matrix, minimum, i, j);
        i = std::get<0>(indices), j = std::get<1>(indices);
        printf("Coordinates: (%d,%d)\n",j,i);
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

    printf("  ");
    for(int i = 0; i < ROWS; i ++)
        printf("%c ", mString1.at(i));
    printf("\n");

    for(int i = 0; i < COLS; i++) {
        printf("%c ", mString2.at(i));
        for(int j = 0; j < ROWS; j++) {
            printf("%d ",matrix[i][j]);
        }
        printf("\n");
    }

    return matrix;
}

int main() {
    //printAlignment(generateMatrix("kitten", "sitting"));
    printAlignment(generateMatrix("Saturday", "Sunday"));


    return 0;
}