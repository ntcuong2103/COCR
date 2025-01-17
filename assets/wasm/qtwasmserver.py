#!/usr/bin/env python3
#############################################################################
##
## Copyright (C) 2021 The Qt Company Ltd.
## Contact: https://www.qt.io/licensing/
##
## This file is part of the plugins of the Qt Toolkit.
##
## $QT_BEGIN_LICENSE:GPL-EXCEPT$
## Commercial License Usage
## Licensees holding valid commercial Qt licenses may use this file in
## accordance with the commercial license agreement provided with the
## Software or, alternatively, in accordance with the terms contained in
## a written agreement between you and The Qt Company. For licensing terms
## and conditions see https://www.qt.io/terms-conditions. For further
## information use the contact form at https://www.qt.io/contact-us.
##
## GNU General Public License Usage
## Alternatively, this file may be used under the terms of the GNU
## General Public License version 3 as published by the Free Software
## Foundation with exceptions as appearing in the file LICENSE.GPL3-EXCEPT
## included in the packaging of this file. Please review the following
## information to ensure the GNU General Public License requirements will
## be met: https://www.gnu.org/licenses/gpl-3.0.html.
##
## $QT_END_LICENSE$
##
#############################################################################

import argparse
import os
import ssl
from http.server import ThreadingHTTPServer, SimpleHTTPRequestHandler
from subprocess import run

import netifaces as ni


# This script implements a web server which serves the content of the current
# working directory using the http and secure https protocols. The server is
# intented to be used as a development server.
#
# Https certificates are generated using the 'mkcert' utility. You should generate
# a certificate authority first, see the mkcert documentation at
# https://github.com/FiloSottile/mkcert
#
# The server sets the COOP and COEP headers, which are required to enable multithreading.

def main():
    parser = argparse.ArgumentParser(
        description="Run a minimal HTTP(S) server to test Qt for WebAssembly applications.",
        formatter_class=argparse.ArgumentDefaultsHelpFormatter,
    )
    parser.add_argument(
        "--port",
        "-p",
        help="Port on which to listen for HTTP and HTTPS (PORT + 1)",
        type=int,
        default=8000,
    )
    parser.add_argument(
        "--address",
        "-a",
        help="Address on which to listen for HTTP and HTTPS, in addition to localhost",
        action="append",
    )
    parser.add_argument(
        "--all",
        help="Start web server which binds to all local interfaces, instead of locahost only",
        action="store_true",
    )
    parser.add_argument(
        "path", help="The directory to serve", nargs="?", default=os.getcwd()
    )

    args = parser.parse_args()
    http_port = args.port
    https_port = http_port + 1
    all_addresses = args.all
    cmd_addresses = args.address or []
    serve_path = args.path

    addresses = ["localhost"] + cmd_addresses
    if all_addresses:
        addresses += [
            addr[ni.AF_INET][0]["addr"]
            for addr in map(ni.ifaddresses, ni.interfaces())
            if ni.AF_INET in addr
        ]
    addresses = sorted(set(addresses))  # deduplicate

    # Generate a https certificate for "localhost" and selected addresses. This
    # requires that the mkcert utility is installed, and that a certificate
    # authority key pair (rootCA-key.pem and rootCA.pem) has been generated. The
    # certificates are written to /tmp, where the https server can find them
    # later on.
    cert_base_path = "/tmp/qtwasmserver-certificate"
    cert_file = f"{cert_base_path}.pem"
    cert_key_file = f"{cert_base_path}-key.pem"
    addresses_string = f"localhost {' '.join(addresses)}"
    ret = run(
        f"mkcert -cert-file {cert_file} -key-file {cert_key_file} {addresses_string}",
        shell=True,
    )
    has_certificate = ret.returncode == 0
    if not has_certificate:
        print(
            "Warning: mkcert is not installed or was unable to create a certificate. Will not start HTTPS server."
        )

    # Http request handler which sends headers required to enable multithreading using SharedArrayBuffer.
    class MyHTTPRequestHandler(SimpleHTTPRequestHandler):
        def __init__(self, request, client_address, server):
            super().__init__(request, client_address, server, directory=serve_path)

        def end_headers(self):
            self.send_header("Cross-Origin-Opener-Policy", "same-origin")
            self.send_header("Cross-Origin-Embedder-Policy", "require-corp")
            self.send_header("Cross-Origin-Resource-Policy", "cross-origin")
            SimpleHTTPRequestHandler.end_headers(self)

    # Serve cwd from http(s)://address:port, with certificates from certdir if set
    def serve_on_thread(address, port, secure):
        httpd = ThreadingHTTPServer((address, port), MyHTTPRequestHandler)
        if secure:
            httpd.socket = ssl.wrap_socket(
                httpd.socket,
                certfile=cert_file,
                keyfile=cert_key_file,
                server_side=True,
            )
        httpd.serve_forever()

    # Start servers
    print(f"Serving at:")
    for address in addresses:
        print(f"  http://{address}:{http_port}")
        serve_on_thread(address, http_port, False)

    if has_certificate:
        for address in addresses:
            print(f"  https://{address}:{https_port}")
            serve_on_thread(address, https_port, True)


if __name__ == "__main__":
    main()