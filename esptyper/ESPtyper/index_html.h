const char *htmlPage = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
    <title>ESP Typer</title>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <style>
        body {
            font-family: Arial, sans-serif;
            margin: 20px;
            background: #f0f0f0;
        }
        .container {
            max-width: 600px;
            margin: 0 auto;
            background: white;
            padding: 20px;
            border-radius: 8px;
            box-shadow: 0 2px 4px rgba(0,0,0,0.1);
        }
        h1 {
            color: #333;
            text-align: center;
        }
        textarea {
            width: 100%;
            min-height: 150px;
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
            width: 100%;
        }
        button:hover {
            background: #0056b3;
        }
        #status {
            margin-top: 10px;
            padding: 10px;
            border-radius: 4px;
            display: none;
        }
        .success {
            background: #d4edda;
            color: #155724;
        }
        .error {
            background: #f8d7da;
            color: #721c24;
        }
    </style>
</head>
<body>
    <div class="container">
        <h1>ESP Typer</h1>
        <textarea id="textInput" placeholder="Enter text here..."></textarea>
        <button onclick="sendText()">Type Text</button>
        <div id="status"></div>
    </div>

    <script>
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
        // 直接将文本作为 URL 参数传递
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
        showStatus('Error: ' + error.message, true);
    }
}
        // 添加快捷键支持
        document.addEventListener('keydown', function(e) {
            // Ctrl+Enter 或 Cmd+Enter 发送文本
            if ((e.ctrlKey || e.metaKey) && e.key === 'Enter') {
                sendText();
            }
        });
    </script>
</body>
</html>
)rawliteral";
