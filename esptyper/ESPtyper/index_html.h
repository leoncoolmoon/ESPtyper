const char *htmlPage = R"rawliteral(
<!DOCTYPE html>
<html>

<head>
    <title>ESP Typer 1.5</title>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <meta charset="UTF-8">
    <style>
        html, body {
            overscroll-behavior: contain;
        }
        body {
            /* font-family: Arial, sans-serif; */
            /*margin: 20px;*/
            background: #f0f0f0;
            min-width: 360px;
        }

        .container {
            max-width: 800px;
            margin: 0 auto;
            background: white;
            padding: 20px;
            border-radius: 8px;
            box-shadow: 0 2px 4px rgba(0, 0, 0, 0.1);
        }

        h1 {
            color: #333;
            text-align: center;
        }

        textarea {
            width: -webkit-fill-available;
            min-height: 5vh;
            height: 20vh;
            padding: 10px;
            margin: 10px 0;
            border: 1px solid #ddd;
            border-radius: 4px;
            font-size: 16px;
            resize: vertical;
        }

        button {
            background: #007bff;
            color: white;
            border: none;
            padding: 10px 20px;
            border-radius: 4px;
            cursor: pointer;
            font-size: 16px;
            margin-right: 5px;
        }

        button:hover {
            background: #0056b3;
        }

        #status {
            margin-top: 10px;
            padding: 10px;
            border-radius: 4px;
            display: none;
            position: absolute;
            top: 50px;
            left: 50px;
            z-index: 9999;
        }

        .success {
            background: #d4edda;
            color: #155724;
        }

        .error {
            background: #f1b0b5a2;
            color: #721c24;
        }

        #typer {
            margin-top: 20px;
            white-space: nowrap;
            overflow-x: auto;
            display: block;
        }

        #mouse {
            display: none;
            margin-top: 20px;
            white-space: nowrap;
            overflow-x: auto;
            text-align: center;
        }

        #touchPad {
            box-shadow: 0 2px 4px rgba(0, 0, 0, 0.1);
            min-height: 5vh;
            height: 20vh;
            min-width: 20vw;
            max-width: 80vw;
            padding: 10px;
            margin: 10px 0;
            border: 1px solid #ddd;
            border-radius: 4px;
            font-size: 16px;
            resize: vertical;
        }

        #mouseButtonContainer {
            margin-bottom: 10px;
            /* 和触控板的间距 */
            display: flex;
            /* 使用 flexbox 排列子元素 */
            justify-content: center;
            /* 水平居中按钮 */
        }

        #mouseButton {
            display: inline-flex;
            flex-direction: column;
            justify-content: center;
            align-items: center;
        }

        #keyboard {
            display: none;
            margin-top: 20px;
            white-space: nowrap;
            overflow-x: auto;
        }

        .keyboard-row {
            text-align: center;
            margin: 2px 0;
        }

        .key {
            display: inline-flex;
            flex-direction: column;
            justify-content: center;
            align-items: center;
            min-width: 3.2vw;
            height: 50px;
            margin: 2px;
            padding: 5px;
            background: #f8f9fa;
            border: 1px solid #dee2e6;
            border-radius: 4px;
            cursor: pointer;
            user-select: none;
            position: relative;
            transition: all 0.2s;
            vertical-align: top;
        }

        .key:active {
            background:
                /*#e9ecef*/
                #007bff;
        }

        .key-sub {
            font-size: 10px;
            color: #666;
            margin-bottom: 2px;
        }

        .key-main {
            font-size: 14px;
            font-weight: bold;
        }

        .space-key {
            min-width: 12vw;
        }

        @media (max-width: 600px) {
            .container {
                margin: 10px;
                padding: 10px;
            }

            .key {

                height: 40px;
                margin: 1px;
                padding: 3px;
            }

            .key-sub {
                font-size: 8px;
            }

            .key-main {
                font-size: 12px;
            }
        }
    </style>
</head>

<body>
    <div class="container">
        <h1>ESP Typer</h1>
        <div id="status"></div>
        <button onclick="toggleView(0)">Typer</button>
        <button onclick="toggleView(1)">Keyboard</button>
        <button onclick="toggleView(2)">Mouse</button>
        <div id="typer">
            <textarea id="textInput" placeholder="Enter text here..."></textarea><br>
            <button onclick="sendText()">Type Text</button>
        </div>
        <div id="mouse">
            <div id="mouseButtonContainer" class="mouse-button-container">
                <button class="mouseButton" id="mouseLeftButton">L</button>
                <button class="mouseButton" id="mouseMiddleButton">M</button>
                <button class="mouseButton" id="mouseRightButton">R</button>
                <button class="mouseButton" id="mouseBackwardButton">◄</button>
                <button class="mouseButton" id="mouseForwardButton">►</button>
            </div>
            <canvas id="touchPad"> </canvas>
        </div>
        <div id="keyboard"></div>

    </div>
    <script>
        const keyboardLayout = [
            [
                { main: '`', sub: '~', type: 0 },
                { main: '1', sub: '!', type: 0 },
                { main: '2', sub: '@', type: 0 },
                { main: '3', sub: '#', type: 0 },
                { main: '4', sub: '$', type: 0 },
                { main: '5', sub: '%', type: 0 },
                { main: '6', sub: '^', type: 0 },
                { main: '7', sub: '&', type: 0 },
                { main: '8', sub: '*', type: 0 },
                { main: '9', sub: '(', type: 0 },
                { main: '0', sub: ')', type: 0 },
                { main: '-', sub: '_', type: 0 },
                { main: '=', sub: '+', type: 0 }
            ],
            [
                { main: 'Q', sub: '#0', type: 0 },
                { main: 'W', sub: '#1', type: 0 },
                { main: 'E', sub: '#2', type: 0 },
                { main: 'R', sub: '#3', type: 0 },
                { main: 'T', sub: 'F1', type: 0 },
                { main: 'Y', sub: 'F2', type: 0 },
                { main: 'U', sub: 'F3', type: 0 },
                { main: 'I', sub: 'F4', type: 0 },
                { main: 'O', sub: 'F5', type: 0 },
                { main: 'P', sub: 'F6', type: 0 },
                { main: '[', sub: '{', type: 0 },
                { main: ']', sub: '}', type: 0 },
                { main: '\\', sub: '|', type: 0 }
            ],
            [
                { main: 'Caps', sub: '', type: 3 },
                { main: 'A', sub: '#4', type: 0 },
                { main: 'S', sub: '#5', type: 0 },
                { main: 'D', sub: '#6', type: 0 },
                { main: 'F', sub: 'F7', type: 0 },
                { main: 'G', sub: 'F8', type: 0 },
                { main: 'H', sub: 'F9', type: 0 },
                { main: 'J', sub: 'F10', type: 0 },
                { main: 'K', sub: 'F11', type: 0 },
                { main: 'L', sub: 'F12', type: 0 },
                { main: ';', sub: ':', type: 0 },
                { main: '\'', sub: '"', type: 0 },
                { main: '⌫', sub: 'Del', type: 0 }
            ],
            [
                { main: 'Shift', sub: '', type: 1 },
                { main: 'Z', sub: '#7', type: 0 },
                { main: 'X', sub: '#8', type: 0 },
                { main: 'C', sub: '#9', type: 0 },
                { main: 'V', sub: '#+', type: 0 },
                { main: 'B', sub: '#-', type: 0 },
                { main: 'N', sub: '#*', type: 0 },
                { main: 'M', sub: '#/', type: 0 },
                { main: ',', sub: '<', type: 0 },
                { main: '.', sub: '>', type: 0 },
                { main: '▲', sub: 'PgUp', type: 0 },
                { main: '/', sub: '?', type: 0 },
                { main: '↩', sub: '', type: 0 }
            ],
            [
                { main: 'Esc', sub: '', type: 0 },
                { main: 'Tab', sub: '', type: 0 },
                { main: 'Ctl', sub: '', type: 1 },
                { main: 'Alt', sub: '', type: 1 },
                { main: 'PtS', sub: '#.', type: 0 },
                { main: 'SPACE', sub: '', class: 'space-key', type: 0 },
                { main: '◄', sub: 'Home', type: 0 },
                { main: '▼', sub: 'PgDn', type: 0 },
                { main: '►', sub: 'End', type: 0 },
                { main: 'Menu', sub: 'Ins', type: 0 },
            ]
        ];
        function showStatus(message, isError = false) {
            const status = document.getElementById('status');
            status.textContent = message;
            status.style.display = 'block';
            status.className = isError ? 'error' : 'success';
            setTimeout(() => {
                status.style.display = 'none';
            }, 3000);
        }
        async function sendText() {
            const mText = document.getElementById('textInput').value;
            if (!mText.trim()) {
                showStatus('Please enter some text', true);
                return;
            }
            try {
                const response = await fetch(`/type?pContent=${encodeURIComponent(mText)}`, {
                    method: 'GET'
                });

                if (response.ok) {
                    showStatus('Text sent successfully');
                    document.getElementById('textInput').value = "";
                } else {
                    showStatus('Failed to send text', true);
                }
            } catch (error) {
                showStatus('Error: ' + error.message + '\n Failed to send text' + mText, true);
            }
        }
        function toggleView(viewIndex) {
            const typer = document.getElementById('typer');
            const keyboard = document.getElementById('keyboard');
            const mouse = document.getElementById('mouse');
            switch (viewIndex) {
                case 0: {
                    typer.style.display = window.getComputedStyle(typer).display === 'none' ? 'block' : 'none';
                    break;
                }
                case 1: {
                    keyboard.style.display = window.getComputedStyle(keyboard).display === 'none' ? 'block' : 'none';
                    break;
                }
                case 2: {
                    mouse.style.display = window.getComputedStyle(mouse).display === 'none' ? 'block' : 'none';
                    break;
                }
            }
        }
        let touchStartY = 0;
        let keyElement = null;
        let actionKey = null;

        function renderKeyboard() {
            const keyboard = document.getElementById('keyboard');
            const container = document.querySelector('.container');
            container.onmouseup = () => {
                if (actionKey !== null && actionKey.sub) { // 向上滑动
                    sendKey(actionKey.sub, actionKey, keyElement);
                    actionKey = null;
                    event.stopPropagation();
                }
            }
            keyboard.onmouseup = () => {
                if (actionKey !== null && actionKey.sub) { // 向上滑动
                    sendKey(actionKey.sub, actionKey, keyElement);
                    actionKey = null;
                    event.stopPropagation();
                }
            }
            container.ontouchend = () => {
                if (actionKey !== null && actionKey.sub) { // 向上滑动
                    sendKey(actionKey.sub, actionKey, keyElement);
                    actionKey = null;
                    event.stopPropagation();
                }
            }
            keyboard.ontouchend = () => {
                if (actionKey !== null && actionKey.sub) { // 向上滑动
                    sendKey(actionKey.sub, actionKey, keyElement);
                    actionKey = null;
                    event.stopPropagation();
                }
            }

            keyboardLayout.forEach(row => {
                const rowDiv = document.createElement('div');
                rowDiv.className = 'keyboard-row';
                row.forEach(key => {
                    const keyDiv = document.createElement('div');
                    keyDiv.className = `key ${key.class || ''}`;
                    if (key.sub) {
                        const subDiv = document.createElement('div');
                        subDiv.className = 'key-sub';
                        subDiv.textContent = key.sub;
                        keyDiv.appendChild(subDiv);
                    }
                    const mainDiv = document.createElement('div');
                    mainDiv.className = 'key-main';
                    mainDiv.textContent = key.main;
                    keyDiv.appendChild(mainDiv);
                    // Mouse events
                    keyDiv.onmousedown = () => {
                        touchStartY = event.clientY;
                        keyElement = event.target;
                        actionKey = key;
                    };
                    keyDiv.onmouseup = () => {

                        const deltaY = event.clientY - touchStartY;
                        if (deltaY < -10 && actionKey.sub) { // 向上滑动
                            sendKey(actionKey.sub, actionKey, keyElement);
                        } else {
                            sendKey(actionKey.main, actionKey, keyElement);
                        }
                        actionKey = null;
                        event.stopPropagation()
                        return false;
                    };
                    // Touch events
                    keyDiv.ontouchstart = () => {
                        touchStartY = event.touches[0].clientY;
                        keyElement = event.target;
                        actionKey = key;
                    };
                    keyDiv.ontouchend = () => {
                        const deltaY = (event.touches.length == 0 ? event.changedTouches[0].clientY : event.touches[0].clientY) - touchStartY;
                        if (deltaY < -10 && key.sub) { // 向上滑动
                            sendKey(actionKey.sub, actionKey, keyElement);
                        } else {
                            sendKey(actionKey.main, actionKey, keyElement);
                        }
                        actionKey = null;
                        event.stopPropagation()
                        return false;
                    };

                    rowDiv.appendChild(keyDiv);
                });
                keyboard.appendChild(rowDiv);
            });
        }
        function keyNameToKeyCode(keyName) {
            const specialKeyCodes = {
                'SPACE': 0x20,
                '⌫': 0xB2, // BACKSPACE
                '↩': 0xB0, // RETURN
                '▲': 0xDA, // UP_ARROW
                '▼': 0xD9, // DOWN_ARROW
                '◄': 0xD8, // LEFT_ARROW
                '►': 0xD7, // RIGHT_ARROW
                'Esc': 0xB1, // ESC
                'Tab': 0xB3, // TAB
                'Ctl': 0x80, // LEFT_CTRL 1d
                'Alt': 0x82, // LEFT_ALT 38
                'Shift': 0x81, // LEFT_SHIFT 2a
                'Caps': 0xC1, // CAPS_LOCK
                'Menu': 0xED, // MENU
                'PtS': 0xCE, // PRINT_SCREEN
                'Ins': 0xD1, // INSERT
                'Home': 0xD2, // HOME
                'PgUp': 0xD3, // PAGE_UP
                'PgDn': 0xD6, // PAGE_DOWN
                'End': 0xD5, // END
                'Del': 0xD4, // DELETE
                'F1': 0xC2,
                'F2': 0xC3,
                'F3': 0xC4,
                'F4': 0xC5,
                'F5': 0xC6,
                'F6': 0xC7,
                'F7': 0xC8,
                'F8': 0xC9,
                'F9': 0xCA,
                'F10': 0xCB,
                'F11': 0xCC,
                'F12': 0xCD,
                '#0': 0xEA, // NUMPAD 0
                '#1': 0xE1, // NUMPAD 1
                '#2': 0xE2, // NUMPAD 2
                '#3': 0xE3, // NUMPAD 3
                '#4': 0xE4, // NUMPAD 4
                '#5': 0xE5, // NUMPAD 5
                '#6': 0xE6, // NUMPAD 6
                '#7': 0xE7, // NUMPAD 7
                '#8': 0xE8, // NUMPAD 8
                '#9': 0xE9, // NUMPAD 9
                '#*': 0xDD, // NUMPAD ASTERISK
                '#+': 0xDF, // NUMPAD PLUS
                '#-': 0xDE, // NUMPAD MINUS
                '#.': 0xEB, // NUMPAD DOT
                '#/': 0xDC  // NUMPAD SLASH
            };
            if (keyName in specialKeyCodes) {
                console.log(keyName);
                console.log(specialKeyCodes[keyName]);
                //console.log(parseInt(specialKeyCodes[keyName], 16));
                return specialKeyCodes[keyName];
            }
            //captial letter
            return keyName.toLowerCase().charCodeAt(0);
        }
        let activateModifyKeyMap = new Map();
        async function sendKey(keyName, key, mkeyDiv) {
            // If the key is empty, do nothing
            if (keyName == "") { return; }
            if (mkeyDiv.className !== "key ") { mkeyDiv = mkeyDiv.parentNode; }
            // Get the background color of the key
            // if type is 0, it's a normal key,
            //if type is 1,2, it's a control key,when pressed odd time key pressed down will be sent and key will show as locked, when pressed  even time key released will be sent and key will show as unlocked
            //if type is 3,4, it's a lock key ,when pressed odd time key will show as locked, when pressed  even time key will show as unlocked
            if (key.type !== 0) {
                //use active key color as locked key color
                //1 ->2 2->1 3->4 4->3
                key.type = key.type > 2 ? 7 - key.type : 3 - key.type;
                mkeyDiv.style.background = key.type % 2 == 0 ? "#007bff" : "#f8f9fa";
                //if type is 2, add the key to the activateModifyKeyMap
                //if type is 1,change the key background to unlocked
                if (key.type == 2) {
                    activateModifyKeyMap.set(keyName, { keyArry: key, keyDiv: mkeyDiv });
                } else if (key.type == 1) {
                    activateModifyKeyMap.get(keyName).keyDiv.style.background = "#f8f9fa";
                    activateModifyKeyMap.get(keyName).keyArry.type = 1;
                }

            } else {
                //change all the key in the activateModifyKeyMap to unlocked
                activateModifyKeyMap.forEach((value, key) => {
                    value.keyDiv.style.background = "#f8f9fa";
                    value.keyArry.type = 1;
                });
            }
            // keyName to keyCode
            keyName = keyNameToKeyCode(keyName);
            // Send the key to the server
            try {
                console.log("pKey= " + keyName);
                console.log("pReleased= " + (key.type % 2 ? "true" : "false"));
                const response = await fetch(`/type?pKey=${encodeURIComponent(keyName)
                    + ((key.type === 0 ||
                        //key.type === 1 || 
                        //key.type === 2 ||
                        key.type === 3 ||
                        key.type === 4
                    ) ? "" : "&pReleased=" + (key.type % 2 ? "true" : "false"))}`
                    , { method: 'GET' });

                if (!response.ok) {
                    showStatus('Failed to send key', true);
                }
            } catch (error) {
                showStatus('Error: ' + error.message + '\n Failed to send key' + keyName, true);
            }
        }
        renderKeyboard();
        function mouseActionToKeyCode(action) {
            const actionMap = {
                'Left': 0x01,//pA 0 = button up, 1 = button down, 2 = button click
                'Right': 0x02,//pA 0 = button up, 1 = button down
                'Forward': 0x10,//pA 2 = button click
                'Backward': 0x08,//pA 2 = button click
                'Middle': 0x04,//pA 2 = button click
                'Scroll': 0x1A,//need further action in MCU, pX and pY
                'Touch': 0x1B//need further action in MCU, pX and pY
            };
            if (action in actionMap) {
                return actionMap[action];
            }
        }
        var actionLock = false;
        document.getElementById('mouseLeftButton').addEventListener('mousedown', (event) => {
            event.stopPropagation();
            event.preventDefault();
            sendMouse('Left', { press: 1 });
        }, { passive: false });
        document.getElementById('mouseLeftButton').addEventListener('mouseup', (event) => {
            event.stopPropagation();
            event.preventDefault();
            sendMouse('Left', { press: 0 });
        }, { passive: false });
        document.getElementById('mouseRightButton').addEventListener('mousedown', (event) => {
            event.stopPropagation();
            event.preventDefault();
            sendMouse('Right', { press: 1 });
        }, { passive: false });
        document.getElementById('mouseRightButton').addEventListener('mouseup', (event) => {
            event.stopPropagation();
            event.preventDefault();
            sendMouse('Right', { press: 0 });
        }, { passive: false });
        document.getElementById('mouseForwardButton').addEventListener('click', () => {
            sendMouse('Forward', { press: 2 });
        }, { passive: false });
        document.getElementById('mouseBackwardButton').addEventListener('click', () => {
            sendMouse('Backward', { press: 2 });
        }, { passive: false });
        let scrollX = 0;
        let scrollY = 0;
        let middleClick = false;
        let startTime = 0;
        let threashold = 10;
        let threasholdTime = 250;
        var mBt = document.getElementById('mouseMiddleButton');
        mBt.addEventListener('mousedown', (event) => {
            event.stopPropagation();
            event.preventDefault();
            middleClick = true;
            scrollY = event.clientY;
            scrollX = event.clientX;
            startTime = Date.now();
        }, { passive: false });
        mBt.addEventListener('mouseup', (event) => {
            event.stopPropagation();
            event.preventDefault();
            if (middleClick) {
                //if{0,0} -> click
                sendMouse('Scroll', { x: Math.abs(scrollX - event.clientX) > threashold ? event.clientX - scrollX : 0, y: Math.abs(scrollY - event.clientY) > threashold ? event.clientY - scrollY : 0 });
            }
            middleClick = false;
        }, { passive: false });
        mBt.addEventListener('mouseleave', (event) => {
            event.preventDefault();
            if (middleClick) {
                sendMouse('Scroll', { x: Math.abs(scrollX - event.clientX) > threashold ? 20 * (scrollX > event.clientX ? -1 : 1) : 0, y: Math.abs(scrollY - event.clientY) > threashold ? 20 * (scrollY > event.clientY ? -1 : 1) : 0 });
                middleClick = false;
            }
        }, { passive: false });
        
        mBt.addEventListener('touchstart', (event) => {
            event.stopPropagation();
            event.preventDefault();
            middleClick = true;
            scrollY = event.Touches.length > 0 ? event.touches[0].clientY : event.changedTouches[0].clientY;
            scrollX = event.touches.length > 0 ? event.touches[0].clientX : event.changedTouches[0].clientX;
            startTime = Date.now();
        }, { passive: false });
        mBt.addEventListener('touchend', (event) => {
            event.stopPropagation();
            event.preventDefault();
            if (middleClick) {
                //if{0,0} -> click
                var cx = event.Touches.length > 0 ? event.touches[0].clientX : event.changedTouches[0].clientX;
                var cy = event.touches.length > 0 ? event.touches[0].clientY : event.changedTouches[0].clientY;
                sendMouse('Scroll',{x:Math.abs(scrollX - cx)>threashold?cx-scrollX:0,y:Math.abs(scrollY - cy)>threashold?cy-scrollY:0});
            }
            middleClick = false;
        }, { passive: false }); 
        mBt.addEventListener('touchcancel', (event) => {
            event.stopPropagation();
            event.preventDefault();
            if (middleClick) {
                var cx = event.Touches.length > 0 ? event.touches[0].clientX : event.changedTouches[0].clientX;
                var cy = event.touches.length > 0 ? event.touches[0].clientY : event.changedTouches[0].clientY;
                sendMouse('Scroll',{x:Math.abs(scrollX - cx)>threashold?20 * (scrollX > cx ? -1 : 1):0,y:Math.abs(scrollY - cy)>threashold?20 * (scrollY > cy ? -1 : 1) :0});
                middleClick = false;
            }
        }, { passive: false });
        mBt.addEventListener('touchmove', (event) => {
            event.stopPropagation();
            event.preventDefault();
            if (middleClick) {
                var cx = event.touches.length > 0 ? event.touches[0].clientX : event.changedTouches[0].clientX;
                var cy = event.touches.length > 0 ? event.touches[0].clientY : event.changedTouches[0].clientY;
                sendMouse('Scroll', { x: Math.abs(scrollX - cx) > threashold ? cx - scrollX : 0, y: Math.abs(scrollY - cy) > threashold ? cy - scrollY : 0 });
            }
        })
        let touchStart = false;
        let touchX = 0;
        let touchY = 0;
        var mTp = document.getElementById('touchPad');
        let mTpw = mTp.offsetWidth;
        let mTph = mTp.offsetHeight;
        mTp.addEventListener('mousedown', (event) => {
            event.stopPropagation();
            event.preventDefault();
            touchStart = true;
            touchX = event.clientX;
            touchY = event.clientY;
            startTime = Date.now();
        }, { passive: false });
        mTp.addEventListener('mouseup', (event) => {
            event.stopPropagation();
            event.preventDefault();
            if (touchStart) {
                //if{0,0} -> click
                sendMouse('Touch', { x: Math.abs(touchX - event.clientX) > threashold ? touchX - event.clientX : 0, y: Math.abs(touchY - event.clientY) > threashold ? touchY - event.clientY : 0 });
            }
            touchStart = false;
        }, { passive: false });
        mTp.addEventListener('mouseleave', (event) => {
            event.stopPropagation();
            event.preventDefault();
            if (touchStart) {
                //if{0,0} -> click
                //sendMouse('Touch', { x: Math.abs(touchX - event.clientX) > threashold ? 20 * (touchX > event.clientX ? -1 : 1) : 0, y: Math.abs(touchY - event.clientY) > threashold ? 20 * (touchY > event.clientY ? -1 : 1) : 0 });
                touchStart = false;
            }
        }, { passive: false });

        mTp.addEventListener('mousemove', (event) => {
            event.stopPropagation();
            event.preventDefault();
            if (touchStart) {
                sendMouse('Touch', { x: touchX !== event.clientX ? touchX - event.clientX : 0, y: touchY !== event.clientY ? touchY - event.clientY : 0 });
                touchX = event.clientX;
                touchY = event.clientY;
            }

        }, { passive: false });
        mTp.addEventListener('touchstart', (event) => {
            event.stopPropagation();
            event.preventDefault();
            touchStart = true;
            touchX = event.touches.length > 0 ? event.touches[0].clientX : event.changedTouches[0].clientX;
            touchY = event.touches.length > 0 ? event.touches[0].clientY : event.changedTouches[0].clientY;
            startTime = Date.now();
        }, { passive: false });
        mTp.addEventListener('touchend',(event) => {
            event.stopPropagation();
            event.preventDefault();
            if (touchStart) {
                //if{0,0} -> click
                var cx = event.touches.length > 0 ? event.touches[0].clientX : event.changedTouches[0].clientX;
                var cy = event.touches.length > 0 ? event.touches[0].clientY : event.changedTouches[0].clientY;
                sendMouse('Touch',{x:Math.abs(touchX - cx)>threashold?touchX-cx:0,y:Math.abs(touchY - cy)>threashold?touchY-cy:0});
            }
            touchStart = false;
        }, { passive: false });
        mTp.addEventListener('touchmove',(event) => {
            event.stopPropagation();
            event.preventDefault();
            if (touchStart) {
                var cx = event.touches.length > 0 ? event.touches[0].clientX : event.changedTouches[0].clientX;
                var cy = event.touches.length > 0 ? event.touches[0].clientY : event.changedTouches[0].clientY;
                sendMouse('Touch',{x:touchX !== cx?touchX-cx:0,y:touchY !== cy?touchY-cy:0});
                touchX = cx;
                touchY = cy;
            }
        }, { passive: false });
        mTp.addEventListener('touchcancel', (event) => {
            event.stopPropagation();
            event.preventDefault();
            if (touchStart) {
                touchStart = false;
            }
        }, { passive: false });
        async function sendMouse(mouseAction, data) {
            try {
                let response = null;
                console.log("action= " + mouseAction);
                if (mouseAction === 'Scroll') {
                    if (data.x === 0 && data.y === 0) {
                        // if (Date.now() - startTime < threasholdTime) {
                        //     return;
                        // }
                        // response = await fetch(`/type?pAction=${mouseActionToKeyCode('Middle')}&pA=2`, { method: 'GET' });
                        // if (!response.ok) {
                        //     showStatus('Failed to send mouse actions', true);
                        // }
                        return;
                    }

                    console.log("pX= " + data.x);
                    console.log("pY= " + data.y);
                    response = await fetch(`/type?pAction=${mouseActionToKeyCode('Scroll')}&pX=${encodeURIComponent(data.x)}&pY=${encodeURIComponent(data.y)}`, { method: 'GET' });

                } else if (mouseAction === 'Touch') {
                    if (data.x === 0 && data.y === 0) {
                        // if (Date.now() - startTime > threasholdTime) {
                        //     return;
                        // }
                        // response = await fetch(`/type?pAction=${mouseActionToKeyCode('Left')}&pA=2`, { method: 'GET' });
                        // if (!response.ok) {
                        //     showStatus('Failed to send mouse actions', true);
                        // }
                        return;
                    }

                    console.log("pX= " + data.x);
                    console.log("pY= " + data.y);
                    data.x = mTpw - data.x;
                    data.y = mTph - data.y;
                    response = await fetch(`/type?pAction=${mouseActionToKeyCode('Touch')}&pX=${encodeURIComponent(data.x)}&pY=${encodeURIComponent(data.y)}`, { method: 'GET' });
                } else {
                    console.log("pA= " + data.press);
                    response = await fetch(`/type?pAction=${mouseActionToKeyCode(mouseAction)}&pA=${data.press}`, { method: 'GET' });
                }
                if (!response.ok) {
                    showStatus('Failed to send mouse actions', true);
                }
            } catch (error) {
                showStatus('Error: ' + error.message + '\n Failed to send mouse', true);
            }
        }
    </script>
</body>
</html>
)rawliteral";
