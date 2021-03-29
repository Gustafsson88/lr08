// Copyright 2021 Danil Postvaykin <postvaykin01@mail.ru>

#include <HTTP_Client.hpp>

HTTP_Client::HTTP_Client(){}

void HTTP_Client::create_req()
{
  std::cout << "Input id" << std::endl;
  std::string str = "";
  std::cin >> str;
  require = "\"input\":" + str;
  std::cout << require << std::endl;
}

void HTTP_Client::start()
{
  auto const host = "127.0.0.1";
  auto const port = "8080";
  auto const target = "/v1/api/suggest";

  net::io_context ioc;

  net::ip::tcp::resolver resolver(ioc);
  beast::tcp_stream stream(ioc);
  auto const results = resolver.resolve(host, port);
  stream.connect(results);

  int version = 11;
  http::request<http::string_body> req{http::verb::post,
                                       target, version};
  req.body() = require;
  req.prepare_payload();
  req.set(http::field::host, host);
  req.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);

  http::write(stream, req);
  beast::flat_buffer buffer;

  http::response<http::dynamic_body> res;
  http::read(stream, buffer, res);
  std::cout << res << std::endl;

  beast::error_code ec;
  stream.socket().shutdown(net::ip::tcp::socket::shutdown_both, ec);

  if (ec && ec != beast::errc::not_connected)
    throw beast::system_error{ec};
}

HTTP_Client::~HTTP_Client(){}
