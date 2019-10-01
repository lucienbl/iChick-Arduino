#include "Router.h"

Router::Router() = default;

String Router::route(const String& header) {
    m_Header = header;

    if (isPath("/login")) {
        return R"({ "path": "login" })";
    }

    return R"({ "status": "error" })";
}

bool Router::isPath(const String& path) {
    return m_Header.indexOf(path) != -1;
}

