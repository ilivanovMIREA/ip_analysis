#include <iostream>
#include <cassert>
#include <cstdlib>
#include <string>
#include <vector

/*
Целью задания является разработка ПО для анализа информации о трафике, прошедшим через некое сетевое устройство. Все хосты-участники считаются наблюдаемым сегментом сети.

Данные
Информация находятся в файле traf.txt. Каждая строка содержит одну запись о сессии передачи данных между двумя узлами наблюдаемого сегмента сети. Поля в строке отделены символом ';'.

Поля данных (по порядку в строке, слева-направо)

IP:Port хоста с которого отправлены данные
MAC хоста с которого отправлены данные

IP:Port хоста на который отправлены данные
MAC хоста на который отправлены данные

Признак UDP: 'true' - UDP, 'false' - TCP

Размер переданных данных, байт
Время передачи, секунд

Пример записи (одна строка): 94.239.149.85:46889;eF:Eb:4e:8:A3:08;161.223.117.183:7306;Ce:62:c5:00:BE:cd;false;10707;0.0941

Задание
Разработайте ПО котрое проанализирует информацию в файле и ответит на следующие вопросы, в отношении наблюдаемого сегмента сети:

Сколько уникальных узлов в наблюдаемой сети (штук)?
Какова средняя скорость передачи данных всей наблюдаемой сети (байт/сек)?
Основываясь на данных о трафике, верно-ли утверждение "UDP используется для передачи данных с максимальной пиковой скоростью"?
Укажите 10 узлов сети с самой высокой средней скоростью передачи данных (байт/сек).
Укажите 10 самых активных подсетей /24 (A.B.C.xxx) по критерию количества сессий передачи данных (штук).
Есть-ли в сети узлы, которые могут являться посредниками (т.н. PROXY) между другими узлами? Укажите их, если они есть. Узел может считаться PROXY если он принимал и передавал данные как минимум двум другим узлам по одному и тому-же протоколу, каждому, т.е., например узлу 1 передавал и принимал по TCP, а узлу 2 по UDP. Прием по одному протоколу, а отправка по другому, одному и тому-же узлу, не допускается в работе PROXY.
Записи с некорректными данными нужно не учитывать в расчетах.

Требования к выполнению задания
Можно использовать любые программные инструменты разработки (языки, библиотеки, IDE и т.п.)
Нельзя использовать готовое ПО для анализа данных, т.е. все расчеты должны быть выполнены самописным кодом
Результат выполнения задания
Результат должен содержать следующие артефакты:

Исходный код, который был создан для выполнения задания и дающий ответ на вопросы задания, путем вывода на экран
Инструкция по запуску исходного кода + перечень необходимых программных компонент для его запуска
В случае использования платформы с компилируемым языком высокого уровня (С/C++/C#/golang/Java и т.п.) требуется приложить скопмилированный исполняемый файл или файл промежуточного представления (Java class/jar и т.п.)*/
//test
#include <algorithm>    // std::copy_if, std::distance

using ip_v = std::vector<std::string>;

auto split(const std::string &str, char d)
{
    ip_v r;

    auto start = 0;
    auto stop = str.find_first_of(d);
    while(stop != std::string::npos)
    {
        r.push_back(str.substr(start, stop - start));

        start = stop + 1;
        stop = str.find_first_of(d, start);
    }

    r.push_back(str.substr(start));

    return r;
}



void ip_print(std::vector<std::vector<std::string>> &ip_filter){
     for(auto ip = 0; ip < ip_filter.size(); ip++){
        for(auto ip_part = ip_filter[ip].cbegin(); ip_part != ip_filter[ip].cend(); ++ip_part)
        {
            if (ip_part != ip_filter[ip].cbegin())
            {
                std::cout << ";";
            }
            std::cout << *ip_part;
        }
        std::cout << std::endl;
    }
}

void ip_print_int(std::vector<std::vector<int>> &ip_filter){
     for(auto ip = 0; ip < ip_filter.size(); ip++){
        for(std::vector<int>::const_iterator ip_part = ip_filter[ip].cbegin(); ip_part != ip_filter[ip].cend(); ++ip_part)
        {
            if (ip_part != ip_filter[ip].cbegin())
            {
                std::cout << ".";
            }
            std::cout << *ip_part;
        }
        std::cout << std::endl;
    }
}

auto getAllIp(std::vector<std::vector<std::string>> &iter_pool){    
    std::vector<std::vector<int>> ip_pool_int1;
    std::vector<std::vector<int>> ip_pool_int2;
    for(auto ip = iter_pool.begin(); ip != iter_pool.end(); ++ip){
        
        
        ip_v temp = split((*ip)[0], ':');
        std::vector<std::string> one = split(temp[0], '.');
        
        std::vector<int> temp_int;
        temp_int.push_back(std::stoi(one[0],nullptr,10));
        temp_int.push_back(std::stoi(one[1],nullptr,10));
        temp_int.push_back(std::stoi(one[2],nullptr,10));
        temp_int.push_back(std::stoi(one[3],nullptr,10));
        ip_pool_int1.push_back(temp_int); 
    
        ip_v temp2 = split((*ip)[2], ':');
        std::vector<std::string> two = split(temp2[0], '.');
        
        std::vector<int> temp_int2;
        temp_int2.push_back(std::stoi(two[0],nullptr,10));
        temp_int2.push_back(std::stoi(two[1],nullptr,10));
        temp_int2.push_back(std::stoi(two[2],nullptr,10));
        temp_int2.push_back(std::stoi(two[3],nullptr,10));
        ip_pool_int2.push_back(temp_int2);
    } 
    
    std::sort(begin(ip_pool_int1), end(ip_pool_int1), std::greater<std::vector<int>>());
    auto last = std::unique(ip_pool_int1.begin(), ip_pool_int1.end());
    ip_pool_int1.erase(last, ip_pool_int1.end()); 
    return ip_pool_int1.size();
}

auto getSpeed(std::vector<std::vector<std::string>> &iter_pool){
    std::vector<std::string> ip_speed;
    float timer = 0;
    int temp = 0;
    
    for(auto speed = iter_pool.begin(); speed != iter_pool.end(); ++speed){
        temp += std::stoi(((*speed)[5]),nullptr,10);
        timer += std::stof (((*speed)[6]),nullptr);
        
    }
        
    double sp = temp/timer;
    
    return sp;
}

auto getMaxUDPSpeed(std::vector<std::vector<std::string>> &iter_pool){
    std::string ch;
    double t;
    double temp = 0;
    for(auto u = iter_pool.begin(); u != iter_pool.end(); ++u){
        
        t = std::stoi(((*u)[5]),nullptr,10)/std::stof (((*u)[6]),nullptr);
        if(temp < t){
            temp = t;
            ch = (*u)[4];
        }
    }
    
    return ch;
}




auto firstTenNode(std::vector<std::vector<std::string>> &iter_pool){
    std::vector<std::vector<int>> ip_pool_int1;
    std::vector<std::vector<int>> ip_pool_int2;
    for(auto ip = iter_pool.begin(); ip != iter_pool.end(); ++ip){
        
        
        ip_v temp = split((*ip)[0], ':');
        std::vector<std::string> one = split(temp[0], '.');
        
        std::vector<int> temp_int;
        temp_int.push_back(std::stoi(one[0],nullptr,10));
        temp_int.push_back(std::stoi(one[1],nullptr,10));
        temp_int.push_back(std::stoi(one[2],nullptr,10));
        temp_int.push_back(std::stoi(one[3],nullptr,10));
        ip_pool_int1.push_back(temp_int); 
    
        ip_v temp2 = split((*ip)[2], ':');
        std::vector<std::string> two = split(temp2[0], '.');
        
        std::vector<int> temp_int2;
        temp_int2.push_back(std::stoi(two[0],nullptr,10));
        temp_int2.push_back(std::stoi(two[1],nullptr,10));
        temp_int2.push_back(std::stoi(two[2],nullptr,10));
        temp_int2.push_back(std::stoi(two[3],nullptr,10));
        ip_pool_int2.push_back(temp_int2);
    }

    std::sort(begin(ip_pool_int2), end(ip_pool_int2), std::greater<std::vector<int>>());
    ip_print_int(ip_pool_int2); // получили отсортированные ip адресса

    // rsa ssh
    


    



/*

    struct sort_class_mass
    {
        bool operator() (std::vector<int> i, std::vector<int> j)
        { return (i[0] < j[0]);}
    } sort_vecMass;

    std::vector<std::string> ipt;
    
    for(auto ip = iter_pool.begin(); ip != iter_pool.end(); ++ip){
        std::string  two;
        
        ip_v temp = split((*ip)[0], ':');
        std::vector<std::vector<std::string>> one;
        one.push_back(split(temp[0].at(0), '.'));


        std::vector<std::vector<int>> ip_pool_int;
        std::vector<int> temp_int;
        temp_int.push_back(std::stoi((*i)[0],nullptr,10));
        temp_int.push_back(std::stoi((*i)[1],nullptr,10));
        temp_int.push_back(std::stoi((*i)[2],nullptr,10));
        temp_int.push_back(std::stoi((*i)[3],nullptr,10));
        ip_pool_int.push_back(temp_int); 

        
        temp = split((*ip)[2], ':');
        two = temp[0];

        std::cout << one << " " << two << " " << (*ip)[5] << " " << (*ip)[6] << std::endl;


    }
*/

    //sort (myvector.begin(), myvector.end(), sort_vecMass);
    return 0;
}

int main(int argc, char const *argv[])
{
    try
    {
        std::vector<std::vector<std::string>> ip_pool;
        
        for(std::string line; std::getline(std::cin, line);)
        {
                ip_v v = split(line, ';');                
                ip_pool.push_back(v);
        }

        std::cout << "Всего строк в файле " << ip_pool.size() << std::endl; 
        //ip_print(ip_pool);      
        std::cout << "Уникальных узлов сети, без портов: " << getAllIp(ip_pool) << std::endl;
        std::cout << "Cредняя скорость передачи данных всей наблюдаемой сети (байт/сек): " << getSpeed(ip_pool) << std::endl;
        std::cout << "Основываясь на данных о трафике, верно-ли утверждение UDP используется для передачи данных с максимальной пиковой скоростью?: " << getMaxUDPSpeed(ip_pool) << std::endl;
        firstTenNode(ip_pool);
    }
    catch(const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
