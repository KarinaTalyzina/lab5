#include <iostream>
#include <vector>
#include <fstream>

// ����� ��� ������� ����������� ��������� ���������������� ������� ��������
class HeatConduction1D {
private:
    int N;                   // ���������� �����
    double L;                // ������� ��������
    double lambda;           // ����������� ����������������
    double rho;              // ��������� ���������
    double c;                // �������� ������������
    double T0;               // ��������� �����������
    double Tl;               // ����������� �� ����� ����
    double Tr;               // ����������� �� ������ ����
    double t_end;            // �����, �� �������� ����� �������
    double h;                // ��� �� ���������������� ����������
    double tau;              // ��� �� �������
    std::vector<double> T;   // ������ ��� �������� ����������
    std::vector<double> alpha, beta;  // ����������� ������������

public:
    // ����������� ������������� ���������� ������
    HeatConduction1D(int nodes, double length, double conductivity, double density, double heatCapacity,
                     double initialTemp, double leftTemp, double rightTemp, double endTime)
        : N(nodes), L(length), lambda(conductivity), rho(density), c(heatCapacity),
          T0(initialTemp), Tl(leftTemp), Tr(rightTemp), t_end(endTime) {
        
        h = L / (N - 1);            // ������ ���� �� ������������
        tau = t_end / 100.0;       // ������ ��� �� �������
        T.resize(N, T0);           // �������������� ������ ���������� ���������� ����������
        alpha.resize(N, 0.0);      // ������ ������������� alpha ��� ������ ��������
        beta.resize(N, 0.0);       // ������ ������������� beta ��� ������ ��������
    }

    // ����� ��� ���������� ���������� ������� ��������� ����������������
    void solve() {  
        double time = 0.0;
        while (time < t_end) {
            time += tau;
            
            // ������������� ��������� �������
            alpha[0] = 0.0;
            beta[0] = Tl;

            // ��������� ����������� ������������ alpha � beta ��� ���������� �������
            for (int i = 1; i < N - 1; ++i) {
                double ai = lambda / (h * h);
                double bi = 2.0 * lambda / (h * h) + rho * c / tau;
                double ci = ai; // �����������: ai � ci �����
                double fi = -rho * c * T[i] / tau;

                alpha[i] = ai / (bi - ci * alpha[i - 1]);
                beta[i] = (ci * beta[i - 1] - fi) / (bi - ci * alpha[i - 1]);
            }

            // ��������� ������ ��������� �������
            T[N - 1] = Tr;

            // �������� �������� ��� ����������� �����������
            for (int i = N - 2; i >= 0; --i) {
                T[i] = alpha[i] * T[i + 1] + beta[i];
            }
        }
    }

    // ����� ��� ������ ����������� � ����
    void saveResults(const std::string &filename) const {
        std::ofstream file(filename);
        if (file.is_open()) {
            file << "������������� ����������� �� ������ ������� t = " << t_end << " ������:\n";
            for (int i = 0; i < N; ++i) {
                file << "x = " << i * h << ", T = " << T[i] << " C\n";
            }
            file.close();
            std::cout << "���������� ��������� � ���� " << filename << std::endl;
        } else {
            std::cerr << "������ �������� ����� ��� ������!" << std::endl;
        }
    }
};

// �������� �������
int main() {
    // ���� ���������� ������
    int N;
    double L, lambda, rho, c, T0, Tl, Tr, t_end;

    std::cout << "������� ���������� ����� �� ���������������� ����������, N: ";
    std::cin >> N;
    std::cout << "������� �������� �����, t_end (�): ";
    std::cin >> t_end;
    std::cout << "������� ������� ��������, L (�): ";
    std::cin >> L;
    std::cout << "������� ����������� ����������������, lambda (��/(�*�)): ";
    std::cin >> lambda;
    std::cout << "������� ���������, rho (��/�^3): ";
        std::cin >> rho;
    std::cout << "������� �������� ������������, c (��/(��*�)): ";
    std::cin >> c;
    std::cout << "������� ��������� �����������, T0 (�C): ";
    std::cin >> T0;
    std::cout << "������� ����������� �� ����� ����, Tl (�C): ";
    std::cin >> Tl;
    std::cout << "������� ����������� �� ������ ����, Tr (�C): ";
    std::cin >> Tr;

    // ������� ������ ������ HeatConduction1D � ������ ������
    HeatConduction1D heatConduction(N, L, lambda, rho, c, T0, Tl, Tr, t_end);
    heatConduction.solve();

    // ��������� ���������� � ����
    heatConduction.saveResults("results.txt");

    return 0; // ���������� ���������
}
