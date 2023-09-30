#include <bits/stdc++.h>
#include <unistd.h>
#include <fstream>
#include <fcntl.h>
#include <random>
#include <chrono>

using namespace std;

mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());

char *read_string(size_t &string_len)
{
    string_len = 0;
    size_t string_cap = 1;
    char *string = new char[string_cap];
    char c;
    read(STDIN_FILENO, &c, sizeof(char));
    if (c == '\n')
        read(STDIN_FILENO, &c, sizeof(char));
    if (c == EOF)
        return nullptr;
    while (c != '\n')
    {
        string[string_len] = c;
        string_len++;
        if (string_len >= string_cap)
        {
            string_cap *= 2;
            char *new_string = new char[string_cap];
            for (size_t i = 0; i < string_len; i++)
            {
                new_string[i] = string[i];
            }
            delete[] string;
            string = new_string;
        }
        read(STDIN_FILENO, &c, sizeof(char));
        if (c == EOF)
            return nullptr;
    }
    string[string_len] = '\0';
    return string;
}

int main()
{
    string file_name_1;
    char symbol_1;

    if (read(STDIN_FILENO, &symbol_1, sizeof(char)) == -1)
    {
        return 7;
    }

    while (symbol_1 != '\n')
    {
        file_name_1 += symbol_1;
        if (read(STDIN_FILENO, &symbol_1, sizeof(char)) == -1)
        {
            return 7;
        }
    }
    int file_1 = open(file_name_1.c_str(), O_WRONLY | O_CREAT, 0777);

    if (file_1 == -1)
    {
        return -1;
    }

    string file_name_2;
    char symbol_2;

    if (read(STDIN_FILENO, &symbol_2, sizeof(char)) == -1)
    {
        return 7;
    }

    while (symbol_2 != '\n')
    {
        file_name_2 += symbol_2;
        if (read(STDIN_FILENO, &symbol_2, sizeof(char)) == -1)
        {
            return 7;
        }
    }
    int file_2 = open(file_name_2.c_str(), O_WRONLY | O_CREAT, 0777);

    if (file_2 == -1)
    {
        return -1;
    }

    int fd_1[2]; // fd[0] - read; fd[1] - write
    // Creating pipe and checking that it was created
    if (pipe(fd_1) == -1)
    {
        return 1;
    }

    int fd_2[2]; // fd[0] - read; fd[1] - write
    // Creating pipe and checking that it was created
    if (pipe(fd_2) == -1)
    {
        return 1;
    }

    int id_1 = fork();
    if (id_1 == -1) // fork error
    {
        return 2;
    }
    else if (id_1 > 0) // in 1 process
    {
        // close(fd_1[0]);

        int id_2 = fork();

        if (id_2 == -1) // fork error
        {
            return 2;
        }
        else if (id_2 > 0)
        {

            size_t len = 0;
            char *stroka = read_string(len);

            char p = 's'; // start

            while (stroka != nullptr)
            {
                if (rng() % 100 <= 80)
                {
                    write(fd_1[1], &p, 1);
                    write(fd_1[1], &len, sizeof(len));
                    write(fd_1[1], stroka, sizeof(char) * len);
                }
                else
                {
                    write(fd_2[1], &p, 1);
                    write(fd_2[1], &len, sizeof(len));
                    write(fd_2[1], stroka, sizeof(char) * len);
                }
                stroka = read_string(len);
            }
            p = 'e'; // end
            write(fd_1[1], &p, 1);
            write(fd_2[1], &p, 1);
            close(file_1);
            close(file_2);
            close(fd_1[1]);
            close(fd_1[0]);
            close(fd_2[1]);
            close(fd_2[0]);
        }
        else
        {
            close(fd_2[1]);
            close(fd_1[1]);
            close(fd_1[0]);
            close(file_1);
            if (dup2(file_2, STDOUT_FILENO) == -1)
            {
                return -1;
            }
            close(file_2);

            if (dup2(fd_2[0], STDIN_FILENO) == -1)
            {
                return -1;
            }
            close(fd_2[0]);

            execl("./son.out", NULL);
            return 0;
        }
    }
    else
    {
        close(fd_1[1]);
        close(fd_2[0]);
        close(fd_2[1]);
        close(file_2);
        if (dup2(file_1, STDOUT_FILENO) == -1)
        {
            return -1;
        }
        close(file_1);
        if (dup2(fd_1[0], STDIN_FILENO) == -1)
        {
            return -1;
        }
        close(fd_1[0]);

        execl("./son.out", NULL);
        return 0;
    }
    return 0;
}
