#ifndef _ASIO_SERVER_HPP_
#define _ASIO_SERVER_HPP_

#include <string.h>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>

using boost::asio::ip::tcp;
using boost::asio::ip::address;

class server_session: public boost::enable_shared_from_this<server_session> {
public:
    server_session(boost::asio::io_service &io_service): socket_(io_service)
    { 
    } 

    void start() {
        static tcp::no_delay option(true);
        socket_.set_option(option);

        boost::asio::async_read_until(socket_, 
                sbuf_,
                "\n",
                boost::bind(&server_session::handle_read, 
                    shared_from_this(), 
                    boost::asio::placeholders::error,
                    boost::asio::placeholders::bytes_transferred));
    } 

    tcp::socket &socket() {
        return socket_;
    } 

private:
    void handle_write(const boost::system::error_code& error, size_t bytes_transferred) {
        boost::asio::async_read_until(socket_, 
                sbuf_,
                "\n",
                boost::bind(&server_session::handle_read, 
                    shared_from_this(), 
                    boost::asio::placeholders::error,
                    boost::asio::placeholders::bytes_transferred));
    } 

    void handle_read(const boost::system::error_code& error, size_t bytes_transferred) {
        boost::asio::async_write(socket_, 
                sbuf_,
                boost::bind(&server_session::handle_write, 
                    shared_from_this(), 
                    boost::asio::placeholders::error,
                    boost::asio::placeholders::bytes_transferred));
    }

private:
    tcp::socket socket_;
    boost::asio::streambuf sbuf_;
};

typedef boost::shared_ptr<server_session> server_session_ptr;

class AsioServer {
public:
        AsioServer(boost::asio::io_service &io_service, tcp::endpoint &endpoint)
            : io_service_(io_service), acceptor_(io_service, endpoint)
        {
            server_session_ptr new_session(new server_session(io_service_));
            acceptor_.async_accept(new_session->socket(),
                boost::bind(&AsioServer::handle_accept,
                    this,
                    new_session,
                    boost::asio::placeholders::error));
        }

        void handle_accept(server_session_ptr new_session, const boost::system::error_code& error) {
            if (error) {
                return;
            }

            new_session->start();
            new_session.reset(new server_session(io_service_));
            acceptor_.async_accept(new_session->socket(),
                boost::bind(&AsioServer::handle_accept,
                    this,
                    new_session,
                    boost::asio::placeholders::error));
        }

        void run() {
            io_service_.run();
        }

private:
        boost::asio::io_service &io_service_;
        tcp::acceptor acceptor_;
};

#endif