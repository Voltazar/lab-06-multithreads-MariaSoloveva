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
#include <boost/log/sinks/text_ostream_backend.hpp>




void Logging()
{
    // Construct the sink
    typedef boost::log::sinks::synchronous_sink<boost::log::sinks::text_ostream_backend> text_sink;
    boost::shared_ptr< text_sink> sink = boost::make_shared< text_sink >();

    // Add a stream to write log to
    sink->locked_backend()->add_stream(
            boost::make_shared< std::ofstream >("sample.log"));

    // Register the sink in the logging core
    boost::log::core::get()->add_sink(sink);

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
    if (args == 2)
    {
        M = atoi(argv[1]);
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
