#ifndef TRANSMITTER_WEB_PAGE_H
#define TRANSMITTER_WEB_PAGE_H

#include "Arduino.h"

const char page[] PROGMEM = R"rawLiteral(
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1, maximum-scale=1, user-scalable=no">
    <title>trim</title>
    <style>
        body {
            margin: 0;
            padding: 0;
        }

        #root {
            display: flex;
            height: 100vh;
        }

        #root>div {
            width: 100%;
            display: flex;
            flex-direction: column;
            justify-content: center;
            align-items: center;
        }

        #root>div:nth-child(1) {
            background-color: #3498db;
        }

        #root>div:nth-child(2) {
            background-color: #444444;
        }

        button {
            height: 4rem;
            width: 6rem;
            margin: 1rem;
            border: 0;
            border-radius: 2rem;
        }

        .toast {
            position: fixed;
            top: 20px;
            right: 20px;
            background-color: #333;
            color: white;
            padding: 10px 20px;
            border-radius: 5px;
            box-shadow: 0 2px 5px rgba(0, 0, 0, 0.2);
            opacity: 0;
            transition: opacity 0.1s ease-in-out;
            z-index: 1000;
        }

        .show {
            opacity: 1;
        }

        #throttle {
            text-align: center;
            font-size: xx-large;
            font-weight: bold;
            color: rgb(255, 255, 255);
            position: fixed;
            top: 1rem;
            left: 50%;
            transform: translateX(-50%);
            z-index: 9999;
        }
    </style>
</head>

<body>
    <div id="toast" class="toast"></div>
    <div id="throttle">100</div>
    <section id="root">
        <div>
            <button onclick="throttlei(this)">up</button>
            <div>
                <button onclick="yawd(this)">left</button>
                <button onclick="reset1(this)">reset</button>
                <button onclick="yawi(this)">right</button>
            </div>
            <button onclick="throttled(this)">down</button>
        </div>
        <div>
            <button onclick="pitchi(this)">up</button>
            <div>
                <button onclick="rolld(this)">left</button>
                <button onclick="reset2(this)">reset</button>
                <button onclick="rolli(this)">right</button>
            </div>
            <button onclick="pitchd(this)">down</button>
        </div>
    </section>
    <script>
        const url = window.location.origin

        const fetchThrottle = () => {
            fetch(`${url}/throttle`) //http://192.168.4.1/throttle
                .then(response => {
                    if (!response.ok) throw new Error('Network response was not ok')
                    return response.text()
                })
                .then(data => {
                    document.getElementById('throttle').textContent = data
                    console.log('Response:', data)
                }).catch(error => console.error('Error:', error))
        }

        const showToast = (message) => {
            const toast = document.getElementById('toast');
            toast.textContent = message;
            toast.classList.add('show');
            setTimeout(() => {
                toast.classList.remove('show');
            }, 800);
        };
        const fetchData = (action) => {
            return fetch(`${url}/action?q=${action}`) //http://192.168.4.1/action?q=ri
                .then(response => {
                    if (!response.ok) throw new Error('Network response was not ok')
                    return response.text()
                })
                .then(data => {
                    showToast(`event: ${data}`)
                    console.log('Response:', data)
                }).catch(error => console.error('Error:', error))
        }
        const throttlei = async (button) => {
            button.disabled = true
            await fetchData('ti')
            await fetchThrottle()
            button.disabled = false
        }
        const throttled = async (button) => {
            button.disabled = true
            await fetchData('td')
            await fetchThrottle()
            button.disabled = false
        }
        const rolli = async (button) => {
            button.disabled = true
            await fetchData('ri')
            button.disabled = false
        }
        const rolld = async (button) => {
            button.disabled = true
            await fetchData('rd')
            button.disabled = false
        }
        const pitchi = async (button) => {
            button.disabled = true
            await fetchData('pi')
            button.disabled = false
        }
        const pitchd = async (button) => {
            button.disabled = true
            await fetchData('pd')
            button.disabled = false
        }
        const yawi = async (button) => {
            button.disabled = true
            await fetchData('yi')
            button.disabled = false
        }
        const yawd = async (button) => {
            button.disabled = true
            await fetchData('yd')
            button.disabled = false
        }
        const reset1 = async (button) => {
            button.disabled = true
            await fetchData('r1')
            await fetchThrottle()
            button.disabled = false
        }
        const reset2 = async (button) => {
            button.disabled = true
            await fetchData('r2')
            button.disabled = false
        }
        fetchThrottle()
    </script>
</body>
</html>
)rawLiteral";

#endif //TRANSMITTER_WEB_PAGE_H
