#include "Filter.h"

Filter::Filter(MyString name, const double matrix1[MATRIX_SIZE][MATRIX_SIZE], double normKff)
{

}
void Filter::apply(Image* image) {

};

ContrastFilter::ContrastFilter() : Filter("Contrast", contrastMatrix)
{
};

BlurFilter::BlurFilter() : Filter("Blur", blurMatrix, 1./9) {

}
