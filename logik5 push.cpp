#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <locale.h>

int main() {
    setlocale(LC_ALL, "Russian");
    srand((unsigned int)time(NULL));

    int n, edgeProb, loopProb;

    printf("Введите количество вершин графа n: ");
    if (scanf("%d", &n) != 1 || n <= 0) {
        printf("Ошибка: количество вершин должно быть положительным числом.\n");
        return 1;
    }

    printf("Введите вероятность появления ребра (от 0 до 100): ");
    if (scanf("%d", &edgeProb) != 1 || edgeProb < 0 || edgeProb > 100) {
        printf("Ошибка: вероятность должна быть в диапазоне от 0 до 100.\n");
        return 1;
    }

    printf("Введите вероятность появления петли на главной диагонали (от 0 до 100): ");
    if (scanf("%d", &loopProb) != 1 || loopProb < 0 || loopProb > 100) {
        printf("Ошибка: вероятность должна быть в диапазоне от 0 до 100.\n");
        return 1;
    }

    // ==================== ЗАДАНИЕ 1 ====================
    printf("\n==================== ЗАДАНИЕ 1 ====================\n");

    int** adjMatrix = (int**)malloc(n * sizeof(int*));
    for (int i = 0; i < n; i++) {
        adjMatrix[i] = (int*)malloc(n * sizeof(int));
    }
    int* degAdj = (int*)calloc(n, sizeof(int));
    int* adjVerticesAdj = (int*)calloc(n, sizeof(int)); // Число смежных ДРУГИХ вершин (без учета петель)

    // 1. Генерация матрицы смежности с учетом вероятностей
    for (int i = 0; i < n; i++) {
        // Генерация петли на главной диагонали
        adjMatrix[i][i] = ((rand() % 100) < loopProb) ? 1 : 0;

        // Генерация обычных ребер
        for (int j = i + 1; j < n; j++) {
            int edge = ((rand() % 100) < edgeProb) ? 1 : 0;
            adjMatrix[i][j] = edge;
            adjMatrix[j][i] = edge;
        }
    }

    // Вывод матрицы смежности
    printf("\nМатрица смежности графа G:\n    ");
    for (int j = 0; j < n; j++) {
        printf("v%-2d ", j + 1);
    }
    printf("\n");

    for (int i = 0; i < n; i++) {
        printf("v%-2d ", i + 1);
        for (int j = 0; j < n; j++) {
            printf(" %-2d ", adjMatrix[i][j]);
        }
        printf("\n");
    }

    // 2. Определение размера графа и степеней вершин по матрице смежности
    int sizeAdj = 0;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (adjMatrix[i][j] == 1) {
                if (i == j) {
                    degAdj[i] += 2; // В неориентированном графе петля дает +2 к степени вершины
                    sizeAdj++;      // Петля — это тоже 1 ребро в размере графа
                }
                else {
                    degAdj[i]++;
                    adjVerticesAdj[i]++; // Учитываем связь именно с другой вершиной
                    if (j > i) {
                        sizeAdj++;
                    }
                }
            }
        }
    }

    printf("\nРазмер графа |E(G)| (по матрице смежности): %d\n", sizeAdj);

    printf("\nСтепени вершин (по матрице смежности, петля дает +2 к степени):\n");
    for (int i = 0; i < n; i++) {
        printf("deg(v%d) = %d (связей с другими вершинами: %d)\n", i + 1, degAdj[i], adjVerticesAdj[i]);
    }

    // 3. Поиск изолированных, концевых и доминирующих вершин
    printf("\nХарактеристики вершин (по матрице смежности):\n");

    int found = 0;
    printf("- Изолированные вершины (нет связей с другими вершинами): ");
    for (int i = 0; i < n; i++) {
        // Проверяем adjVerticesAdj[i] == 0, чтобы петля не учитывалась
        if (adjVerticesAdj[i] == 0) {
            printf("v%d ", i + 1);
            found = 1;
        }
    }
    if (!found) printf("нет");
    printf("\n");

    found = 0;
    printf("- Концевые вершины (deg = 1): ");
    for (int i = 0; i < n; i++) {
        if (degAdj[i] == 1) {
            printf("v%d ", i + 1);
            found = 1;
        }
    }
    if (!found) printf("нет");
    printf("\n");

    found = 0;
    printf("- Доминирующие вершины (смежны со всеми %d другими вершинами): ", n - 1);
    for (int i = 0; i < n; i++) {
        if (n > 1 && adjVerticesAdj[i] == n - 1) {
            printf("v%d ", i + 1);
            found = 1;
        }
    }
    if (!found) printf("нет");
    printf("\n");


    // ==================== ЗАДАНИЕ 2* ====================
    printf("\n==================== ЗАДАНИЕ 2* ===================\n");

    int m = sizeAdj;
    int** incMatrix = NULL;

    if (m > 0) {
        incMatrix = (int**)malloc(n * sizeof(int*));
        for (int i = 0; i < n; i++) {
            incMatrix[i] = (int*)calloc(m, sizeof(int));
        }

        // 1. Построение матрицы инцидентности на основе матрицы смежности
        int edgeIndex = 0;
        for (int i = 0; i < n; i++) {
            for (int j = i; j < n; j++) { // j начинается с i, чтобы захватить главную диагональ (петли)
                if (adjMatrix[i][j] == 1) {
                    if (i == j) {
                        // Для петли в матрице инцидентности ставится 2 (два конца ребра в одной вершине)
                        incMatrix[i][edgeIndex] = 2;
                    }
                    else {
                        incMatrix[i][edgeIndex] = 1;
                        incMatrix[j][edgeIndex] = 1;
                    }
                    edgeIndex++;
                }
            }
        }

        // Вывод матрицы инцидентности
        printf("\nМатрица инцидентности графа G (2 означает петлю):\n    ");
        for (int j = 0; j < m; j++) {
            printf("e%-2d ", j + 1);
        }
        printf("\n");

        for (int i = 0; i < n; i++) {
            printf("v%-2d ", i + 1);
            for (int j = 0; j < m; j++) {
                printf(" %-2d ", incMatrix[i][j]);
            }
            printf("\n");
        }
    }
    else {
        printf("\nВ графе нет ребер (|E(G)| = 0), матрица инцидентности пуста (0 столбцов).\n");
    }

    // 2. Определение размера графа и степеней вершин по матрице инцидентности
    int* degInc = (int*)calloc(n, sizeof(int));
    int* adjVerticesInc = (int*)calloc(n, sizeof(int));
    int sizeInc = 0;

    for (int j = 0; j < m; j++) {
        int colSum = 0;
        for (int i = 0; i < n; i++) {
            if (incMatrix[i][j] > 0) {
                degInc[i] += incMatrix[i][j]; // Прибавляем 1 для обычного ребра или 2 для петли
                colSum += incMatrix[i][j];
                if (incMatrix[i][j] == 1) {
                    adjVerticesInc[i]++; // Только обычные ребра связывают вершину с ДРУГИМИ вершинами
                }
            }
        }
        if (colSum == 2) { // Сумма в столбце равна 2 и для обычного ребра (1+1), и для петли (2)
            sizeInc++;
        }
    }

    printf("\nРазмер графа |E(G)| (по матрице инцидентности): %d\n", sizeInc);

    printf("\nСтепени вершин (по матрице инцидентности):\n");
    for (int i = 0; i < n; i++) {
        printf("deg(v%d) = %d (связей с другими вершинами: %d)\n", i + 1, degInc[i], adjVerticesInc[i]);
    }

    // 3. Поиск изолированных, концевых и доминирующих вершин по матрице инцидентности
    printf("\nХарактеристики вершин (по матрице инцидентности):\n");

    found = 0;
    printf("- Изолированные вершины (нет связей с другими вершинами): ");
    for (int i = 0; i < n; i++) {
        // Проверяем adjVerticesInc[i] == 0, чтобы петля не учитывалась
        if (adjVerticesInc[i] == 0) {
            printf("v%d ", i + 1);
            found = 1;
        }
    }
    if (!found) printf("нет");
    printf("\n");

    found = 0;
    printf("- Концевые вершины (deg = 1): ");
    for (int i = 0; i < n; i++) {
        if (degInc[i] == 1) {
            printf("v%d ", i + 1);
            found = 1;
        }
    }
    if (!found) printf("нет");
    printf("\n");

    found = 0;
    printf("- Доминирующие вершины (смежны со всеми %d другими вершинами): ", n - 1);
    for (int i = 0; i < n; i++) {
        if (n > 1 && adjVerticesInc[i] == n - 1) {
            printf("v%d ", i + 1);
            found = 1;
        }
    }
    if (!found) printf("нет");
    printf("\n");

    // Освобождение памяти
    for (int i = 0; i < n; i++) {
        free(adjMatrix[i]);
        if (incMatrix != NULL) {
            free(incMatrix[i]);
        }
    }
    free(adjMatrix);
    if (incMatrix != NULL) {
        free(incMatrix);
    }
    free(degAdj);
    free(adjVerticesAdj);
    free(degInc);
    free(adjVerticesInc);

    return 0;
}