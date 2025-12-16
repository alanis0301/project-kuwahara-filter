#include "../include/kuwahara.h"
#include <math.h> // Comente para Otimizar MELHORIA 1 (Otimização)


// // Melhoria 1
// void kuwahara_filter(int image[IMG_SIZE][IMG_SIZE], int window)
// {
//     // Define dimensões da imagem e calcular tamanho dos quadrantes
//     int width = IMG_SIZE, height = IMG_SIZE;
//     int window_size = window;
//     int quadrant_size = (window_size + 1) / 2;
//     int result[IMG_SIZE][IMG_SIZE]; // ESTRUTURA DE DADOS auxiliar para armazenar o resultado (8100 valores inteiros)

//     // Percorre cada pixel da imagem
//     for (int pixel_y = 0; pixel_y < height; ++pixel_y)
//     {
//         for (int pixel_x = 0; pixel_x < width; ++pixel_x)
//         {
//             // Calcula canto superior esquerdo da janela centrada no pixel atual
//             int window_top_y = pixel_y - (window_size / 2);
//             int window_left_x = pixel_x - (window_size / 2);

//             // Inicializa busca pelo quadrante com menor desvio padrão
//             // double best_std_dev = 1e300; // valor inicial muito grande // Comente para Otimizar MELHORIA 1 (Otimização)
//             double best_variance = 1e300; // Descomente para Otimizar MELHORIA 1 (Otimização)
//             double best_mean = image[pixel_y][pixel_x];

//             // Analisa os 4 quadrantes sobrepostos
//             // Pykuwahara usa "anchors" na ordem: (0,0), (0,1), (1,0), (1,1)
//             // Que correspondem aos quadrantes: (1,1), (0,1), (1,0), (0,0)
//             int quadrant_order[4][2] = {{1,1}, {0,1}, {1,0}, {0,0}};
            
//             for (int q = 0; q < 4; ++q)
//             {
//                 int quadrant_y = quadrant_order[q][0];
//                 int quadrant_x = quadrant_order[q][1];
                
//                 // Acumula soma e soma dos quadrados para calcular estatísticas
//                 long long sum = 0, sum_sq = 0;
//                 int pixel_count = 0;

//                 // Percorre pixels dentro do quadrante atual
//                 for (int offset_y = 0; offset_y < quadrant_size; ++offset_y)
//                 {
//                     for (int offset_x = 0; offset_x < quadrant_size; ++offset_x)
//                     {
//                         // Calcula posição real do pixel (com sobreposição dos quadrantes)
//                         int read_y = window_top_y + (quadrant_y ? (quadrant_size - 1) : 0) + offset_y;
//                         int read_x = window_left_x + (quadrant_x ? (quadrant_size - 1) : 0) + offset_x;

//                         // Aplica BORDER_REFLECT_101 (reflexão espelhada, como no OpenCV)
//                         // Reflete para dentro da imagem sem incluir o pixel da borda
//                         if (read_y < 0)
//                             read_y = -read_y;
//                         if (read_y >= height)
//                             read_y = 2 * height - read_y - 2;
//                         if (read_x < 0)
//                             read_x = -read_x;
//                         if (read_x >= width)
//                             read_x = 2 * width - read_x - 2;
                        
//                         // Clamping como fallback para casos extremos
//                         if (read_y < 0)
//                             read_y = 0;
//                         if (read_y >= height)
//                             read_y = height - 1;
//                         if (read_x < 0)
//                             read_x = 0;
//                         if (read_x >= width)
//                             read_x = width - 1;

//                         // Acumula valores para cálculo de média e desvio padrão
//                         int pixel_value = image[read_y][read_x];
//                         sum += pixel_value;
//                         sum_sq += (long long)pixel_value * (long long)pixel_value;
//                         pixel_count++;
//                     }
//                 }

//                 // Calcula estatísticas do quadrante atual
//                 if (pixel_count > 1)
//                 {
//                     double mean = (double)sum / (double)pixel_count;
                    
//                     // Calcula Variância Populacional
//                     double variance = ((double)sum_sq - (double)sum * sum / pixel_count) / pixel_count;
                    
//                     //BEGIN 0 MELHORIA 1: Comente para Otimizar (Otimização)
//                     // double std_dev = sqrt(variance); 

//                     // // Atualiza se encontrou quadrante mais homogêneo (menor desvio padrão)
//                     // if (std_dev < best_std_dev)
//                     // {
//                     //     best_std_dev = std_dev;
//                     //     best_mean = mean; // Armazena sem arredondar
//                     // }
//                     //END 0 MELHORIA 1: Comente para Otimizar (Otimização)
                    
//                     //BEGIN 1 MELHORIA 1: Descomente para Otimizar (Otimização)
//                     if (variance < best_variance) {
//                         best_variance = variance;
//                         best_mean = mean;
//                     }
//                     //END 1 MELHORIA 1: Descomente para Otimizar (Otimização)
//                 }
//             }
//             // Atribui média do melhor quadrante ao pixel de saída
//             result[pixel_y][pixel_x] = (int)(best_mean);
//         }
//     }

//     // Copia resultado do buffer temporário para a imagem original
//     for (int i = 0; i < height; ++i)
//     {
//         for (int j = 0; j < width; ++j)
//         {
//             image[i][j] = result[i][j];
//         }
//     }
// }

// // Melhoria 2:
// void kuwahara_filter(int image[IMG_SIZE][IMG_SIZE], int window)
// {
//     int width = IMG_SIZE, height = IMG_SIZE;
//     int window_size = window;

//     const int radius = window_size / 2;
//     const int quadrant_size = (window_size + 1) / 2;
//     const int shift = quadrant_size - 1;
//     const int pixel_count = quadrant_size * quadrant_size;

//     int result[IMG_SIZE][IMG_SIZE];

//     static const int quadrant_order[4][2] = {{1,1}, {0,1}, {1,0}, {0,0}};

//     // MIOLO (sem borda):
//     for (int pixel_y = radius; pixel_y < height - radius; ++pixel_y)
//     {
//         for (int pixel_x = radius; pixel_x < width - radius; ++pixel_x)
//         {
//             const int window_top_y  = pixel_y - radius;
//             const int window_left_x = pixel_x - radius;

//             double best_variance = 1e300;
//             double best_mean = image[pixel_y][pixel_x];

//             // Calcula estatísticas do quadrante atual
//             if (pixel_count > 1)
//             {
//                 for (int q = 0; q < 4; ++q)
//                 {
//                     const int quadrant_y = quadrant_order[q][0];
//                     const int quadrant_x = quadrant_order[q][1];

//                     const int base_y = window_top_y  + (quadrant_y ? shift : 0);
//                     const int base_x = window_left_x + (quadrant_x ? shift : 0);

//                     long long sum = 0, sum_sq = 0;

//                     for (int oy = 0; oy < quadrant_size; ++oy)
//                     {
//                         const int ry = base_y + oy;
//                         for (int ox = 0; ox < quadrant_size; ++ox)
//                         {
//                             const int rx = base_x + ox;
//                             const int v = image[ry][rx];
//                             sum += v;
//                             sum_sq += (long long)v * (long long)v;
//                         }
//                     }

//                     const double mean = (double)sum / (double)pixel_count;
//                     const double variance =
//                         ((double)sum_sq - (double)sum * (double)sum / (double)pixel_count) / (double)pixel_count;

//                     if (variance >= 0.0 && variance < best_variance)
//                     {
//                         best_variance = variance;
//                         best_mean = mean;
//                     }
//                 }
//             }

//             result[pixel_y][pixel_x] = (int)(best_mean);
//         }
//     }

//     // BORDAS:
//     for (int pixel_y = 0; pixel_y < height; ++pixel_y)
//     {
//         for (int pixel_x = 0; pixel_x < width; ++pixel_x)
//         {
//             // pula miolo já processado
//             if (pixel_y >= radius && pixel_y < height - radius &&
//                 pixel_x >= radius && pixel_x < width - radius)
//                 continue;

//             const int window_top_y  = pixel_y - radius;
//             const int window_left_x = pixel_x - radius;

//             double best_variance = 1e300;
//             double best_mean = image[pixel_y][pixel_x];

//             if (pixel_count > 1)
//             {
//                 for (int q = 0; q < 4; ++q)
//                 {
//                     const int quadrant_y = quadrant_order[q][0];
//                     const int quadrant_x = quadrant_order[q][1];

//                     const int base_y = window_top_y  + (quadrant_y ? shift : 0);
//                     const int base_x = window_left_x + (quadrant_x ? shift : 0);

//                     long long sum = 0, sum_sq = 0;

//                     for (int oy = 0; oy < quadrant_size; ++oy)
//                     {
//                         for (int ox = 0; ox < quadrant_size; ++ox)
//                         {
//                             int read_y = base_y + oy;
//                             int read_x = base_x + ox;

//                             // BORDER_REFLECT_101
//                             if (read_y < 0) read_y = -read_y;
//                             if (read_y >= height) read_y = 2 * height - read_y - 2;
//                             if (read_x < 0) read_x = -read_x;
//                             if (read_x >= width) read_x = 2 * width - read_x - 2;

//                             // fallback clamp
//                             if (read_y < 0) read_y = 0;
//                             if (read_y >= height) read_y = height - 1;
//                             if (read_x < 0) read_x = 0;
//                             if (read_x >= width) read_x = width - 1;

//                             const int v = image[read_y][read_x];
//                             sum += v;
//                             sum_sq += (long long)v * (long long)v;
//                         }
//                     }

//                     const double mean = (double)sum / (double)pixel_count;
//                     const double variance =
//                         ((double)sum_sq - (double)sum * (double)sum / (double)pixel_count) / (double)pixel_count;

//                     if (variance >= 0.0 && variance < best_variance)
//                     {
//                         best_variance = variance;
//                         best_mean = mean;
//                     }
//                 }
//             }

//             result[pixel_y][pixel_x] = (int)(best_mean);
//         }
//     }

//     // Copia resultado do buffer temporário para a imagem original
//     for (int i = 0; i < height; ++i) {
//         for (int j = 0; j < width; ++j) {
//             image[i][j] = result[i][j];
//         }
//     }
// }

// Melhoria 3:
void kuwahara_filter(int image[IMG_SIZE][IMG_SIZE], int window)
{
    int width = IMG_SIZE, height = IMG_SIZE;
    int window_size = window;

    const int radius = window_size / 2;
    const int quadrant_size = (window_size + 1) / 2;
    const int shift = quadrant_size - 1;
    const int pixel_count = quadrant_size * quadrant_size;

    int result[IMG_SIZE][IMG_SIZE];

    static const int quadrant_order[4][2] = {{1,1}, {0,1}, {1,0}, {0,0}};

    // MIOLO (sem borda):
    for (int pixel_y = radius; pixel_y < height - radius; ++pixel_y)
    {
        for (int pixel_x = radius; pixel_x < width - radius; ++pixel_x)
        {
            const int window_top_y  = pixel_y - radius;
            const int window_left_x = pixel_x - radius;

            double best_std_dev = 1e300;
            double best_mean = image[pixel_y][pixel_x];

            // Calcula estatísticas do quadrante atual
            if (pixel_count > 1)
            {
                for (int q = 0; q < 4; ++q)
                {
                    const int quadrant_y = quadrant_order[q][0];
                    const int quadrant_x = quadrant_order[q][1];

                    const int base_y = window_top_y  + (quadrant_y ? shift : 0);
                    const int base_x = window_left_x + (quadrant_x ? shift : 0);

                    long long sum = 0, sum_sq = 0;

                    for (int oy = 0; oy < quadrant_size; ++oy)
                    {
                        const int ry = base_y + oy;
                        for (int ox = 0; ox < quadrant_size; ++ox)
                        {
                            const int rx = base_x + ox;
                            const int v = image[ry][rx];
                            sum += v;
                            sum_sq += (long long)v * (long long)v;
                        }
                    }

                    const double mean = (double)sum / (double)pixel_count;
                    const double variance =
                        ((double)sum_sq - (double)sum * (double)sum / (double)pixel_count) / (double)pixel_count;

                    double std_dev = sqrt(variance); 

                    // Atualiza se encontrou quadrante mais homogêneo (menor desvio padrão)
                    if (std_dev < best_std_dev)
                    {
                        best_std_dev = std_dev;
                        best_mean = mean; // Armazena sem arredondar
                    }
                }
            }

            result[pixel_y][pixel_x] = (int)(best_mean);
        }
    }

    // BORDAS:
    for (int pixel_y = 0; pixel_y < height; ++pixel_y)
    {
        for (int pixel_x = 0; pixel_x < width; ++pixel_x)
        {
            // pula miolo já processado
            if (pixel_y >= radius && pixel_y < height - radius &&
                pixel_x >= radius && pixel_x < width - radius)
                continue;

            const int window_top_y  = pixel_y - radius;
            const int window_left_x = pixel_x - radius;

            double best_std_dev = 1e300;
            double best_mean = image[pixel_y][pixel_x];

            if (pixel_count > 1)
            {
                for (int q = 0; q < 4; ++q)
                {
                    const int quadrant_y = quadrant_order[q][0];
                    const int quadrant_x = quadrant_order[q][1];

                    const int base_y = window_top_y  + (quadrant_y ? shift : 0);
                    const int base_x = window_left_x + (quadrant_x ? shift : 0);

                    long long sum = 0, sum_sq = 0;

                    for (int oy = 0; oy < quadrant_size; ++oy)
                    {
                        for (int ox = 0; ox < quadrant_size; ++ox)
                        {
                            int read_y = base_y + oy;
                            int read_x = base_x + ox;

                            // BORDER_REFLECT_101
                            if (read_y < 0) read_y = -read_y;
                            if (read_y >= height) read_y = 2 * height - read_y - 2;
                            if (read_x < 0) read_x = -read_x;
                            if (read_x >= width) read_x = 2 * width - read_x - 2;

                            // fallback clamp
                            if (read_y < 0) read_y = 0;
                            if (read_y >= height) read_y = height - 1;
                            if (read_x < 0) read_x = 0;
                            if (read_x >= width) read_x = width - 1;

                            const int v = image[read_y][read_x];
                            sum += v;
                            sum_sq += (long long)v * (long long)v;
                        }
                    }

                    const double mean = (double)sum / (double)pixel_count;
                    const double variance =
                        ((double)sum_sq - (double)sum * (double)sum / (double)pixel_count) / (double)pixel_count;

                    double std_dev = sqrt(variance); 

                    // Atualiza se encontrou quadrante mais homogêneo (menor desvio padrão)
                    if (std_dev < best_std_dev)
                    {
                        best_std_dev = std_dev;
                        best_mean = mean; // Armazena sem arredondar
                    }
                }
            }

            result[pixel_y][pixel_x] = (int)(best_mean);
        }
    }

    // Copia resultado do buffer temporário para a imagem original
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            image[i][j] = result[i][j];
        }
    }
}