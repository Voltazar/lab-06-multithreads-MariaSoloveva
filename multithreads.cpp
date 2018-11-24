#include <algorithm>
#include <iostream>
#include <exception>
#include <thread>
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <picosha2.h>

#include <boost/log/utility/setup/file.hpp>
#include <boost/log/sinks/sink.hpp>



//  namespace logging = boost::log;

void Logging()
{
    boost::log::core::get()->set_filter
    (
        boost::log::trivial::severity >= boost::log::trivial::trace
        ||  boost::log::trivial::severity >= boost::log::trivial::info
    );
}


void Find(int m)
{
    Logging();
    while(true)
    {
        const std::vector<unsigned char> data_{(unsigned char)std::rand(), (unsigned char)std::rand(),
                                               (unsigned char)std::rand(), (unsigned char)std::rand()};
        const std::string hash = picosha2::hash256_hex_string(data_);
        if (hash.find(("0000"), hash.size() - 5) != std::string::npos)
        {
            BOOST_LOG_TRIVIAL(trace) << "Hash with '0000'" << hash;
        }
        else
        {
            BOOST_LOG_TRIVIAL(info) << "Thread : " << m << " hash : " << hash;
        }
    }
}

int main(int args, char* argv[])
{
    std::srand(time(NULL));
    int M = std::thread::hardware_concurrency();
    std::vector<std::thread> threads;
    args = 2;
    if (args == 2)
    {
        M = atoi(argv[1]);
    }
    else
    {
        throw std::runtime_error("Error");
    }

    for (int m = 0; m < M; ++m)
    {
        threads.emplace_back(Find, m);
    }

    for (int m = 0; m < M; ++m)
    {
        threads[m].join();
    }

    return 0;
}
