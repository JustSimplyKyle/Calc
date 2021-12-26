#include <iostream>
#include <string>
#include <vector>
#include <cmath>

using namespace std;

struct number{
    vector<string> number_str;
    vector<char> input_ch;
    vector<char> math_calc;
    vector<char> pure_calc;
    vector<double> calc_numbers;
};

double factor(double number) {
    double ret = 1;
    while (number > 1) {
        ret *= number--;
    }
    return ret;
}

double small_calc(double left, char ch, double right) {
    double result = 0;
    switch (ch) {
        case '+':
            result = left + right;
            break;
        case '-':
            result = left - right;
            break;
        case '*':
            result = left * right;
            break;
        case '/':
            result = left / right;
            break;
        case '^':
            result = pow(left, right);
            break;
        case '!':
            result = factor(left);
            break;
        default:
            return result;
    }
    return result;
}

void parenthesis(number &Number,char par1,char par2){ //par1 = ( or [ par2= ) or ]
    double big_calc(number number);
    int para_size = 0;
    for (int i = 0; i < Number.math_calc.size(); ++i) {
        if (Number.math_calc[i] == par1) {
            ++para_size;
        }
    }
    if (par1=='(')
        char opsitite='[';
    else if (par1=='[');
        char opsitite='(';
    for (int k = 0; k < para_size; ++k) { // this loop will only excute when there is parenthesis.
        int now_para_size = 0;
        auto in_iter = Number.math_calc.begin();
        auto out_iter = Number.math_calc.begin();
        int num_in = 0;
        int num_out = 0;
        number num;
        for (int i = 0; i <= Number.math_calc.size(); ++i) {
            if (Number.math_calc[i] == par1) {
                in_iter += i;
                for (int j = 0; j <= Number.math_calc.size(); ++j) {
                    if (Number.math_calc[j] == par1) {
                        now_para_size = 0;           //reset now_para_size
                        for (int w = 0; w <= j; ++w) //find how many parenthesis are in the current iteration
                        {
                            if (Number.math_calc[w] == par1 || Number.math_calc[w] == opsitite) {
                                ++now_para_size;
                            }
                        }
                        in_iter = Number.math_calc.begin() + j;
                        num_in = j + 1 - now_para_size;
                    } else if (Number.math_calc[j] == par2) {
                        out_iter += j;
                        num_out = j - now_para_size - 1;
                        break;
                    }
                }
                break;
            }
        }
        for (int i = num_in; i <= num_out; ++i) {
            num.math_calc.push_back(Number.pure_calc[i]);
        }
        for (int i = num_in; i <= num_out + 1; ++i) {
            num.calc_numbers.push_back(Number.calc_numbers[i]);
        }
        if(par1=='[')
            Number.calc_numbers[num_in] = abs(big_calc(num));
        else
            Number.calc_numbers[num_in]=big_calc(num);
        Number.calc_numbers.erase(num_in + Number.calc_numbers.begin() + 1, num_out + Number.calc_numbers.begin() + 2);
        Number.pure_calc.erase(Number.pure_calc.begin() + num_in, num_out + Number.pure_calc.begin() + 1);
        Number.math_calc.erase(in_iter, out_iter + 1);
        // start resetting variables
        num.math_calc.clear();
        num.calc_numbers.clear();
    }
}
double big_calc(number number) {
    parenthesis(number,'(',')');
    double temp_result;
    double result;
    for (int i = 0; i < number.math_calc.size(); ++i) // calculate power first
    {
        if (number.math_calc[i] == '^' || number.math_calc[i] == '!') {
            number.calc_numbers[i] = small_calc(number.calc_numbers[i], number.math_calc[i], number.calc_numbers[i + 1]);
            number.calc_numbers.erase(number.calc_numbers.begin() + i + 1);
            number.math_calc.erase(number.math_calc.begin() + i);
            --i; // to compensate for the erase movement
        }
    }
    for (int i = 0; i < number.math_calc.size(); ++i) // calculate multiplication and division first
    {
        if (number.math_calc[i] == '*' || number.math_calc[i] == '/') {
            number.calc_numbers[i] = small_calc(number.calc_numbers[i], number.math_calc[i], number.calc_numbers[i + 1]);
            number.calc_numbers.erase(number.calc_numbers.begin() + i + 1);
            number.math_calc.erase(number.math_calc.begin() + i);
            --i; // to compensate for the erase movement
        }
    }
    temp_result = number.calc_numbers[0];
    for (int i = 0; i < number.math_calc.size(); ++i) {
        temp_result = small_calc(temp_result, number.math_calc[i], number.calc_numbers[i + 1]);
    }
    result = temp_result;
    return result;
}
int main() {
    cout << "Input Calc" << endl;
    number number;
    string input_temp;
    cin >> input_temp;
    for (char i: input_temp) {
        number.input_ch.push_back(i);
    }
    auto begin_iterator = number.input_ch.begin();
    int cnt = 0;
    for (int i = 0; i < number.input_ch.size(); i++) {
        if (*begin_iterator == '~') {
            *begin_iterator = '^';
            number.input_ch.insert(begin_iterator + 1, '0');
            begin_iterator = number.input_ch.begin() + cnt; //iterators need to be reset after movement
            number.input_ch.insert(begin_iterator + 2, '.');
            begin_iterator = number.input_ch.begin() + cnt;
            number.input_ch.insert(begin_iterator + 3, '5');
            begin_iterator = number.input_ch.begin() + cnt;
        }
        if (*begin_iterator == '!') {
            number.input_ch.insert(begin_iterator + 1, '1');
            begin_iterator = number.input_ch.begin() + cnt;
        }
        if (begin_iterator < number.input_ch.end()) {
            ++cnt;
            ++begin_iterator;
        }
    }
    begin_iterator = number.input_ch.begin();
    auto end_iterator = number.input_ch.end();
    for (int i = 0; i < number.input_ch.size(); ++i) // extract char
    {
        if (!isdigit(*begin_iterator) && *begin_iterator != '.') {
            if (!(!isdigit(*(begin_iterator - 1)) && *begin_iterator == '-')) {
                number.math_calc.push_back(*begin_iterator);
                if (*begin_iterator != '(' && *begin_iterator != ')' && *begin_iterator != '[' && *begin_iterator !=']') {
                    number.pure_calc.push_back(*begin_iterator);
                }
                *begin_iterator = '\n';
            }

        }
        if (begin_iterator < end_iterator) {
            ++cnt;
            ++begin_iterator;
        }
    }
    string str_temp;
    begin_iterator = number.input_ch.begin();
    end_iterator = number.input_ch.end();
    for (int i = 0; i <= number.math_calc.size(); ++i) // extract str_numbers
    {
        while (*begin_iterator != '\n') {
            if (isdigit(*begin_iterator) || *begin_iterator == '.' || *begin_iterator == '-') {
                str_temp.push_back(*begin_iterator);
            }
            if (begin_iterator < end_iterator) {
                ++begin_iterator;
            } else {
                break;
            }
        }
        if (!str_temp.empty())
            number.number_str.push_back(str_temp);
        str_temp.clear();
        ++begin_iterator;
    }
    auto in_it = number.number_str.begin(); //it stands for iterator
    auto out_it = number.number_str.end();
    for (int i = 0; i < number.number_str.size(); ++i) //convert str_numbers to double_numbers
    {
        number.calc_numbers.push_back(stod(*in_it)); //stod -- convert str_number to double
        if (in_it < out_it) {
            ++in_it;
        }
    }
    parenthesis(number,'[',']');
    cout << big_calc(number) << endl;
    return 0;
}
