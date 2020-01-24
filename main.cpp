#include <iostream>
#include <vector>
#include <string>
#include <algorithm>    // find
#include <codecvt>

struct Uri {
public:
    std::wstring QueryString, Path, Protocol, Host, Port;

    static Uri Parse(const std::wstring &uri) {
        Uri result;

        typedef std::wstring::const_iterator iterator_t;

        if (uri.length() == 0)
            return result;

        iterator_t uriEnd = uri.end();

        // get query start
        iterator_t queryStart = std::find(uri.begin(), uriEnd, L'?');

        // protocol
        iterator_t protocolStart = uri.begin();
        iterator_t protocolEnd = std::find(protocolStart, uriEnd, L':');            //"://");

        if (protocolEnd != uriEnd) {
            std::wstring prot = &*(protocolEnd);
            if ((prot.length() > 3) && (prot.substr(0, 3) == L"://")) {
                result.Protocol = std::wstring(protocolStart, protocolEnd);
                protocolEnd += 3;   //      ://
            } else
                protocolEnd = uri.begin();  // no protocol
        } else
            protocolEnd = uri.begin();  // no protocol

        // host
        iterator_t hostStart = protocolEnd;
        iterator_t pathStart = std::find(hostStart, uriEnd, L'/');  // get pathStart

        iterator_t hostEnd = std::find(protocolEnd,
                                       (pathStart != uriEnd) ? pathStart : queryStart,
                                       L':');  // check for port

        result.Host = std::wstring(hostStart, hostEnd);

        // port
        if ((hostEnd != uriEnd) && ((&*(hostEnd))[0] == L':'))  // we have a port
        {
            hostEnd++;
            iterator_t portEnd = (pathStart != uriEnd) ? pathStart : queryStart;
            result.Port = std::wstring(hostEnd, portEnd);
        }

        // path
        if (pathStart != uriEnd)
            result.Path = std::wstring(pathStart, queryStart);

        // query
        if (queryStart != uriEnd)
            result.QueryString = std::wstring(queryStart, uri.end());

        return result;

    }   // Parse
};  // uri


int parse(std::string url) {
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    Uri u = Uri::Parse(converter.from_bytes(url));
    std::string host = converter.to_bytes(u.Host);
    std::string protocol = converter.to_bytes(u.Protocol);
    std::string port = converter.to_bytes(u.Port);
    std::string page = converter.to_bytes(u.Path);
    if (protocol == "")
        protocol = "http";
    if (port == "")
        port = "80";
    if (page == "")
        page = "/";

    std::cout << "Protocol: " << protocol << " Host: " << host << " Port: " << port << " Page: " << page
              << std::endl;
}

int main() {
    std::vector<std::string> urls;
    urls.push_back("http://example.com/index.html");
    urls.push_back("example.com:8080/");
    urls.push_back("example.com");
    for (int i = 0; i < urls.size(); i++) {
        parse(urls[i]);

    }
    std::cout << "vvedite url" << std::endl;
    std::string url = "";
    std::cin >> url;
    parse(url);
    return 0;
}