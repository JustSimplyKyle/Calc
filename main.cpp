#include <iostream>
#include <string>
#include <vector>
#include <cmath>

using namespace std;

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

double big_calc(vector<double> numbers, vector<char> math_calc) {
    double temp_result;
    double result;
    for (int i = 0; i < math_calc.size(); ++i) // calculate power first
    {
        if (math_calc[i] == '^' || math_calc[i] == '!') {
            numbers[i] = small_calc(numbers[i], math_calc[i], numbers[i + 1]);
            numbers.erase(numbers.begin() + i + 1);
            math_calc.erase(math_calc.begin() + i);
            --i; // to compensate for the erase movement
        }
    }
    for (int i = 0; i < math_calc.size(); ++i) // calculate multiplication and division first
    {
        if (math_calc[i] == '*' || math_calc[i] == '/') {
            numbers[i] = small_calc(numbers[i], math_calc[i], numbers[i + 1]);
            numbers.erase(numbers.begin() + i + 1);
            math_calc.erase(math_calc.begin() + i);
            --i; // to compensate for the erase movement
        }
    }
    temp_result = numbers[0];
    for (int i = 0; i < math_calc.size(); ++i) {
        temp_result = small_calc(temp_result, math_calc[i], numbers[i + 1]);
    }
    result = temp_result;
    return result;
}

int main() {
    cout << "Input Calc" << endl;
    vector<string> number_str;
    vector<char> input_ch;
    vector<char> math_calc;
    vector<char> pure_calc;
    vector<double> numbers;
    string input_temp;
    cin >> input_temp;
    for (char i: input_temp) {
        input_ch.push_back(i);
    }
    auto begin_iterator = input_ch.begin();
    int cnt = 0;
    for (int i = 0; i < input_ch.size(); i++) {
        if (*begin_iterator == '~') {
            *begin_iterator = '^';
            input_ch.insert(begin_iterator + 1, '0');
            begin_iterator = input_ch.begin() + cnt; //iterators need to be reset after movement
            input_ch.insert(begin_iterator + 2, '.');
            begin_iterator = input_ch.begin() + cnt;
            input_ch.insert(begin_iterator + 3, '5');
            begin_iterator = input_ch.begin() + cnt;
        }
        if (*begin_iterator == '!') {
            input_ch.insert(begin_iterator + 1, '1');
            begin_iterator = input_ch.begin() + cnt;
        }
        if (begin_iterator < input_ch.end()) {
            ++cnt;
            ++begin_iterator;
        }
    }
    begin_iterator = input_ch.begin();
    auto end_iterator = input_ch.end();
    for (int i = 0; i < input_ch.size(); ++i) // extract char
    {
        if (!isdigit(*begin_iterator) && *begin_iterator != '.') {
            if (!(!isdigit(*(begin_iterator - 1)) && *begin_iterator == '-')) {
                math_calc.push_back(*begin_iterator);
                if (*begin_iterator != '(' && *begin_iterator != ')') {
                    pure_calc.push_back(*begin_iterator);
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
    begin_iterator = input_ch.begin();
    end_iterator = input_ch.end();
    for (int i = 0; i <= math_calc.size(); ++i) // extract str_numbers
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
            number_str.push_back(str_temp);
        str_temp.clear();
        ++begin_iterator;
    }
    auto in_it = number_str.begin(); //it stands for iterator
    auto out_it = number_str.end();
    for (int i = 0; i < number_str.size(); ++i) //convert str_numbers to double_numbers
    {
        numbers.push_back(stod(*in_it)); //stod -- convert str_number to double
        if (in_it < out_it) {
            ++in_it;
        }
    }
    int para_size = 0;
    for (int i = 0; i <= math_calc.size(); ++i) {
        if (math_calc[i] == '(') {
            ++para_size;
        }
    }
    for (int k = 0; k < para_size; k++) {
        int now_para_size = 0;
        auto in_iter = math_calc.begin();
        auto out_iter = math_calc.begin();
        int num_in = 0;
        int num_out = 0;
        vector<double> temp_numbers;
        vector<char> temp_ch;
        for (int i = 0; i <= math_calc.size(); i++) {
            if (math_calc[i] == '(') {
                in_iter += i;
                for (int j = 0; j <= math_calc.size(); j++) {
                    if (math_calc[j] == '(') {
                        now_para_size = 0;           //reset now_para_size
                        for (int w = 0; w <= j; w++) //find how many parenthesis are in the current iteration
                        {
                            if (math_calc[w] == '(') {
                                ++now_para_size;
                            }
                        }
                        in_iter = math_calc.begin() + j;
                        num_in = j + 1 - now_para_size;
                    } else if (math_calc[j] == ')') {
                        out_iter += j;
                        num_out = j - now_para_size - 1;
                        break;
                    }
                }
                break;
            }
        }
        for (int i = num_in; i <= num_out; i++) {
            temp_ch.push_back(pure_calc[i]);
        }
        for (int i = num_in; i <= num_out + 1; i++) {
            temp_numbers.push_back(numbers[i]);
        }
        numbers[num_in] = big_calc(temp_numbers, temp_ch);
        numbers.erase(num_in + numbers.begin() + 1, num_out + numbers.begin() + 2);
        pure_calc.erase(pure_calc.begin() + num_in, num_out + pure_calc.begin() + 1);
        math_calc.erase(in_iter, out_iter + 1);
        // start resetting variables
        temp_ch.clear();
        temp_numbers.clear();
    }
    cout << big_calc(numbers, math_calc) << endl;
    return 0;
}
