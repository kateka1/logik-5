#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <locale.h>

int main() {
    setlocale(LC_ALL, "Russian");
    srand((unsigned int)time(NULL));

    int n;
    printf("Введите количество вершин графа n: ");
    if (scanf("%d", &n) != 1 || n <= 0) { //если буква или <0 - ошибка ахтунг
        printf("Ошибка: количество вершин должно быть положительным числом.\n");
        return 1;
    }

    // задание 1 мат. смеж. (вершины на вершины, 1 где ребра, 0 на гл линии, тк граф неопред.)
    printf("\n==================== ЗАДАНИЕ 1 ====================\n");

    int** adjMatrix = (int**)malloc(n * sizeof(int*)); //двойной указатель, тк компилятор не даст создать матрицу без заранее заданного n
    for (int i = 0; i < n; i++) { //Сверху выделяем непрерывный массив из n ячеек. Каждая ячейка хранит не число, а указатель на строку int*
        adjMatrix[i] = (int*)malloc(n * sizeof(int)); //Для каждого указателя строки выделяем отдельный блок памяти на n ячеек. Это даст нам обращаться станд. через aM[i][j]
    }
    int* degAdj = (int*)calloc(n, sizeof(int)); // выбрал calloc чтобы занулить все байты без лишнего мусора, тк degAdj используем как счетчик

    //1) Генерация матрицы смежности
    for (int i = 0; i < n; i++) {
        adjMatrix[i][i] = 0; //обнуляем гл диаг
        for (int j = i + 1; j < n; j++) { //заполняем верхний треугольник от гл диаг
            int edge = rand() % 2;
            adjMatrix[i][j] = edge; //зеркалим
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

    //2) Определение размера графа и степеней вершин по матрице смежности
    int sizeAdj = 0; //счетчик каждый раз встречая 1 в матрице делает ++
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (adjMatrix[i][j] == 1) {
                degAdj[i]++;
                if (j > i) { //условие чтобы брать только верхний треуг. тк матрица зеркальна относ. гл диаг
                    sizeAdj++;
                }
            }
        }
    }

    printf("\nРазмер графа |E(G)| (по матрице смежности): %d\n", sizeAdj);

    printf("\nСтепени вершин (по матрице смежности):\n");
    for (int i = 0; i < n; i++) {
        printf("deg(v%d) = %d\n", i + 1, degAdj[i]);
    }

    //3) Поиск изолированных, концевых и доминирующих вершин
    printf("\nХарактеристики вершин (по матрице смежности):\n");

    int found = 0;
    printf("- Изолированные вершины (deg = 0): "); //Если степень = 0, в строке i нет 1
    for (int i = 0; i < n; i++) {
        if (degAdj[i] == 0) {
            printf("v%d ", i + 1);
            found = 1;
        }
    }
    if (!found) printf("нет");
    printf("\n");

    found = 0;
    printf("- Концевые вершины (deg = 1): "); //Должно вести только 1 ребро
    for (int i = 0; i < n; i++) {
        if (degAdj[i] == 1) {
            printf("v%d ", i + 1);
            found = 1;
        }
    }
    if (!found) printf("нет");
    printf("\n");

    found = 0;
    printf("- Доминирующие вершины (deg = %d): ", n - 1); //Если степень = n-1 - доминирующая (те все связи кроме петель)
    for (int i = 0; i < n; i++) {
        if (n > 1 && degAdj[i] == n - 1) {
            printf("v%d ", i + 1);
            found = 1;
        }
    }
    if (!found) printf("нет");
    printf("\n");

    // Матрица инцидентности - таблица вершины на ребра, где 1 - связь есть, 0 - связи нет.
    printf("\n==================== ЗАДАНИЕ 2* ===================\n");

    // Количество столбцов матрицы инцидентности равно количеству ребер графа
    int m = sizeAdj; //кол-во ребер = число столбцов
    int** incMatrix = NULL;

    if (m > 0) {
        incMatrix = (int**)malloc(n * sizeof(int*));
        for (int i = 0; i < n; i++) {
            incMatrix[i] = (int*)calloc(m, sizeof(int));
        }

        //1) Построение матрицы инцидентности на основе матрицы смежности
        int edgeIndex = 0;
        for (int i = 0; i < n; i++) {
            for (int j = i + 1; j < n; j++) {
                if (adjMatrix[i][j] == 1) {
                    // Ребро edgeIndex соединяет вершины i и j
                    incMatrix[i][edgeIndex] = 1;
                    incMatrix[j][edgeIndex] = 1;
                    edgeIndex++;
                }
            }
        }

        // Вывод матрицы инцидентности
        printf("\nМатрица инцидентности графа G:\n    ");
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

    //2)Определение размера графа и степеней вершин по матрице инцидентности
    int* degInc = (int*)calloc(n, sizeof(int));
    int sizeInc = 0;

    // В матрице инцидентности каждый столбец — это одно ребро,
    // а сумма единиц в строке i равна степени вершины v_i
    for (int j = 0; j < m; j++) { //сумма 1 в i строке - степень вершины v_i
        int colSum = 0;
        for (int i = 0; i < n; i++) {
            if (incMatrix[i][j] == 1) {
                degInc[i]++;
                colSum++;
            }
        }
        if (colSum == 2) { // Каждое ребро инцидентно двум вершинам
            sizeInc++;
        }
    }

    printf("\nРазмер графа |E(G)| (по матрице инцидентности): %d\n", sizeInc);

    printf("\nСтепени вершин (по матрице инцидентности):\n");
    for (int i = 0; i < n; i++) {
        printf("deg(v%d) = %d\n", i + 1, degInc[i]);
    }

    //3) Поиск изолированных, концевых и доминирующих вершин по матрице инцидентности
    printf("\nХарактеристики вершин (по матрице инцидентности):\n");

    found = 0;
    printf("- Изолированные вершины (deg = 0): ");
    for (int i = 0; i < n; i++) {
        if (degInc[i] == 0) {
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
    printf("- Доминирующие вершины (deg = %d): ", n - 1);
    for (int i = 0; i < n; i++) {
        if (n > 1 && degInc[i] == n - 1) {
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
    free(degInc);

    return 0;
}

// Размер графа - общее кол-во ребер
// Степень вершины - количество ребер выходящих/входящих из вершины
// Изолированная (дег 0) - нет ребер, концевая (дег 1) - к ней ведет только 1 ребро, доминирующая (дег н-1) - соедин. ребром со всеми остальными вершинами графа.