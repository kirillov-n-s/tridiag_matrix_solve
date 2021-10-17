#include <fstream>
#include <string>

#include "tridiag.h"
#include "solve.h"

using real = double;

bool from_file(num::tridiag<real> &mat, num::vector<real> &vec, const std::string &what)
{
    std::cout << "Enter file path: ";
    std::string path;
    std::cin.ignore();
    std::getline(std::cin, path);
    std::ifstream in(path);
    std::cout << std::endl;

    if (!in.is_open())
    {
        std::cout << "File does not exist. Return to main menu." << std::endl << std::endl;
        return false;
    }

    std::size_t n;
    in >> n;
    mat = num::tridiag<real>(n);
    vec = num::vector<real>(n);
    in >> mat >> vec;

    in.close();
    std::cout << "Values successfully read from file." << std::endl << std::endl
              << "Matrix [[A]] is:" << std::endl << mat << std::endl
              << "Vector [" << what << "] is:" << std::endl << vec << std::endl;
    return true;
}

void limits(real &min_a, real &max_a, real &min_b, real &max_b, real &min_c, real &max_c, real &min, real &max, const std::string& what)
{
    std::cout << "Enter vector [a] component range (min max):" << std::endl;
    std::cin >> min_a >> max_a;

    std::cout << "Enter vector [b] component range (min max):" << std::endl;
    std::cin >> min_b >> max_b;

    std::cout << "Enter vector [c] component range (min max):" << std::endl;
    std::cin >> min_c >> max_c;

    std::cout << "Enter vector [" << what << "] component range (min, max):" << std::endl;
    std::cin >> min >> max;
    std::cout << std::endl;
}

bool random(num::tridiag<real> &mat, num::vector<real> &vec, const std::string &what)
{
    std::cout << "Enter system size: ";
    std::size_t n;
    std::cin >> n;
    std::cout << std::endl;

    if (n < 1)
    {
        std::cout << "Size less than 1. Return to main menu." << std::endl << std::endl;
        return false;
    }

    real min_a, max_a, min_b, max_b, min_c, max_c, min, max;
    limits(min_a, max_a, min_b, max_b, min_c, max_c, min, max, what);

    mat = num::tridiag<real>(n, min_a, max_a, min_b, max_b, min_c, max_c);
    vec = num::vector<real>(n, min, max);

    std::cout << "Values successfully randomized." << std::endl << std::endl
              << "Matrix [[A]] is:" << std::endl << mat << std::endl
              << "Vector [" << what << "] is:" << std::endl << vec << std::endl;
    return true;
}

bool fill(num::tridiag<real> &mat, num::vector<real> &vec, const std::string &what)
{
    int choice;
    std::cout << "Select fill method:" << std::endl
              << "\t1 - from file;" << std::endl
              << "\t2 - random;" << std::endl
              << "\tother - exit." << std::endl;
    std::cin >> choice;
    std::cout << std::endl;

    switch (choice)
    {
        case 1:
            return from_file(mat, vec, what);
        case 2:
            return random(mat, vec, what);
        default:
            return false;
    }
}

/*bool solve(const num::tridiag<real> &mat, const num::vector<real> &vec, num::vector<real> &res)
{
    int choice;
    std::cout << "Select numeric method:" << std::endl
              << "\t1 - Thomas algorithm;" << std::endl
              << "\t2 - unstable method;" << std::endl
              << "\tother - exit." << std::endl;
    std::cin >> choice;
    std::cout << std::endl;

    switch (choice)
    {
        case 1:
            res = num::thomas_alg(mat, vec);
            return true;
        case 2:
            res = num::unstable_method(mat, vec);
            return true;
        default:
            return false;
    }
}*/

void solution_mode()
{
    num::tridiag<real> mat;
    num::vector<real> vec, thomas, unstable;

    if (!fill(mat, vec, "d"))
        return;

    thomas = num::thomas_alg(mat, vec);
    unstable = num::unstable_method(mat, vec);

    std::cout << "Result vector [x] from Thomas algorithm is:" << std::endl << thomas << std::endl
              << "Result vector [x] from unstable method is:" << std::endl << unstable << std::endl
              << "Result difference norm ||[x]T - [x]u|| is: " << (thomas - unstable).norm() << std::endl << std::endl;
}

void test_mode()
{
    num::tridiag<real> mat;
    num::vector<real> vec, exact, thomas, unstable;

    if (!fill(mat, exact, "x*"))
        return;

    vec = mat * exact;
    thomas = num::thomas_alg(mat, vec);
    unstable = num::unstable_method(mat, vec);

    std::cout << "Vector [d] = [[A]] * [x*] is:" << std::endl << vec << std::endl
              << "Result vector [x] from Thomas algorithm is:" << std::endl << thomas
              << "Thomas algorithm error ||[x*] - [x]|| is: " << (exact - thomas).norm() << std::endl << std::endl
              << "Result vector [x] from unstable method is:" << std::endl << unstable
              << "Unstable method error ||[x*] - [x]|| is: " << (exact - unstable).norm() << std::endl << std::endl;
}

void error_table()
{
    auto sizes = { 10, 50, 100, 500, 1000, 5000, 10000, 50000, 100000, 500000, 1000000 };

    std::string head_thomas = "Thomas algorithm ERROR",
                head_unstable = "Unstable method ERROR",
                sep = " | ";

    auto places = num::format<real>(std::cout, false);
    std::size_t width_size = std::log10(std::numeric_limits<std::size_t>::max()) + 1,
                width_thomas = std::max(places, head_thomas.size()),
                width_unstable = std::max(places, head_unstable.size());

    real min_a, max_a, min_b, max_b, min_c, max_c, min, max;
    limits(min_a, max_a, min_b, max_b, min_c, max_c, min, max, "x*");

    std::cout << std::setw(width_size) << "SIZE" << sep
              << std::setw(width_thomas) << head_thomas << sep
              << std::setw(width_unstable) << head_unstable << std::endl

              << std::setfill('_') << ' '
              << std::setw(width_size + 1) << "|"
              << std::setw(width_thomas + 3) << "|"
              << std::setw(width_unstable + 1) << "_" << std::endl

              << std::setfill(' ')
              << std::setw(width_size + 3) << sep
              << std::setw(width_thomas + 3) << sep
              << std::endl;

    for (auto size : sizes)
    {
        num::tridiag<real> mat(size, min_a, max_a, min_b, max_b, min_c, max_c);
        num::vector<real> exact(size, min, max);

        auto vec = mat * exact;
        auto thomas = num::thomas_alg(mat, vec);
        auto unstable = num::unstable_method(mat, vec);

        std::cout << std::setw(width_size) << mat.size() << sep
                  << std::setw(width_thomas) << (exact - thomas).norm() << sep
                  << std::setw(width_unstable) << (exact - unstable).norm()
                  << std::endl;
    }

    std::cout << std::endl;
}

int main()
{
    int choice;
    while (true)
    {
        std::cout << "Select mode:" << std::endl
                  << "\t1 - solution mode;" << std::endl
                  << "\t2 - test mode;" << std::endl
                  << "\t3 - error table;" << std::endl
                  << "\tother - exit." << std::endl;

        std::cin >> choice;
        std::cout << std::endl;

        switch (choice)
        {
            case 1:
                solution_mode();
                break;
            case 2:
                test_mode();
                break;
            case 3:
                error_table();
                break;
            default:
                return 0;
        }
    }
}
