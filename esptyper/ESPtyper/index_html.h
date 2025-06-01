const char *htmlPage = R"rawliteral(
<!DOCTYPE html>
<html>

<head>
    <title>ESP Typer 1.6</title>
    <meta name="viewport" content="user-scalable=no,width=device-width, initial-scale=1">
    <meta charset="UTF-8">
    <!-- <script src="ocrad.js"></script> -->
    <style>

        html, body {
             height: 100%;
             overflow: hidden;
             overscroll-behavior: none;
             /*position: fixed;*/
             overflow-y: auto;
        justify-content: center;
        }
        
        body {
            /* font-family: Arial, sans-serif; */
            margin: 5vw;
            background: #f0f0f0;
            min-width: 360px;
        }
        body::after {
          content: '';
          display: block;
          height: 100px; /* 添加一个假内容撑开页面 */
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
          canvas {
            touch-action: none;  
          }

        /* OCR related styles */
        .file-input {
            display: none;
        }
        
        .ocr-preview {
            max-width: 100%;
            max-height: 200px;
            display: none;
            margin: 10px 0;
            border-radius: 4px;
        }
        
        .ocr-progress {
            display: none;
            margin: 10px 0;
            color: #666;
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
        .previewDIV {
      position: fixed;
      top: 0;
      left: 0;
      width: 100%;
      height: 100%;
      background: rgba(0, 0, 0, 0.9);
      display: none;
      justify-content: center;
      align-items: center;
      z-index: 1000;
      overflow: hidden;
      touch-action: none;
    }
    .previewImg{
      max-width: 100%;
      max-height: 100%;
      object-fit: contain;
      transform-origin: 0 0;
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
            <input type="file" id="fileInput" class="file-input" accept="image/*,text/*">
            <button onclick="document.getElementById('fileInput').click()">Open File (OCR)</button>
            <button onclick="sendText()">Type Text</button>
            <img id="ocrPreview" class="ocr-preview" alt="Preview">
            <div id="ocrProgress" class="ocr-progress">Processing...</div>
      <div id="fullscreen-preview" class="previewDIV">
        <img id="preview-image" class="previewImg">
      </div>
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
// 全局变量记录拖动和缩放状态
let isDragging = false;
let isScaling = false;
let startX = 0, startY = 0;
let offsetX = 0, offsetY = 0;
let scale = 1;
let initialDistance = 0;
let currentImage = null;

// 点击 OCR 预览 Canvas 时触发全屏预览
document.getElementById('ocrPreview').addEventListener('click', function(e) {
   
    const previewDiv = document.getElementById('fullscreen-preview');
    const previewImg = document.getElementById('preview-image');
    
    previewDiv.style.display = 'flex';
    currentImage = previewImg;
    
    // 重置缩放和位置
    scale = 1;
    offsetX = 0;
    offsetY = 0;
    updateImageTransform();
});

// 关闭全屏预览的函数
function closePreview() {
    document.getElementById('fullscreen-preview').style.display = 'none';
}

// 更新图片的缩放和位移
function updateImageTransform() {
    if (currentImage) {
        currentImage.style.transform = `translate(${offsetX}px, ${offsetY}px) scale(${scale})`;
    }
}

// 单击关闭（判断是否移动）
document.getElementById('fullscreen-preview').addEventListener('mousedown', function(e) {
    if (e.button !== 0) return; // 仅左键
    startX = e.clientX;
    startY = e.clientY;
    isDragging = false;
});

document.getElementById('fullscreen-preview').addEventListener('mousemove', function(e) {
    if (startX !== 0 && startY !== 0) {
        const dx = e.clientX - startX;
        const dy = e.clientY - startY;
        if (Math.abs(dx) > 5 || Math.abs(dy) > 5) {
            isDragging = true;
            offsetX += dx;
            offsetY += dy;
            startX = e.clientX;
            startY = e.clientY;
            updateImageTransform();
        }
    }
});

document.getElementById('fullscreen-preview').addEventListener('mouseup', function(e) {
    if (!isDragging && e.button === 0) {
        closePreview();
    }
    startX = 0;
    startY = 0;
});

// 触摸事件支持（移动端）
document.getElementById('fullscreen-preview').addEventListener('touchstart', function(e) {
    if (e.touches.length === 1) {
        startX = e.touches[0].clientX;
        startY = e.touches[0].clientY;
        isDragging = false;
    isScaling = false;
    } else if (e.touches.length === 2) {
        // 双指缩放
        initialDistance = Math.hypot(
            e.touches[0].clientX - e.touches[1].clientX,
            e.touches[0].clientY - e.touches[1].clientY
        );
      isScaling = true;
      isDragging = false;
    }
});

document.getElementById('fullscreen-preview').addEventListener('touchmove', function(e) {
    if (e.touches.length === 1 && startX !== 0 && startY !== 0) {
        const dx = e.touches[0].clientX - startX;
        const dy = e.touches[0].clientY - startY;
        if (Math.abs(dx) > 5 || Math.abs(dy) > 5) {
            isDragging = true;
      isScaling = false;
            offsetX += dx;
            offsetY += dy;
            startX = e.touches[0].clientX;
            startY = e.touches[0].clientY;
            updateImageTransform();
        }
    } else if (e.touches.length === 2 && initialDistance > 0) {
        // 双指缩放
        const currentDistance = Math.hypot(
            e.touches[0].clientX - e.touches[1].clientX,
            e.touches[0].clientY - e.touches[1].clientY
        );
    isScaling = true;
    isDragging = false;
        scale = Math.max(0.5, Math.min(3, scale * (currentDistance / initialDistance)));
        initialDistance = currentDistance;
        updateImageTransform();
    }
});

document.getElementById('fullscreen-preview').addEventListener('touchend', function(e) {
    if (e.touches.length === 0 && !isDragging && !isScaling) {
        closePreview();
    isDragging = false;
    isScaling = false;
    }
    startX = 0;
    startY = 0;
    initialDistance = 0;
});

// 滚轮缩放
document.getElementById('fullscreen-preview').addEventListener('wheel', function(e) {
    e.preventDefault();
    const delta = -e.deltaY / 100; // 缩放灵敏度
    scale = Math.max(0.5, Math.min(3, scale + delta));
    updateImageTransform();
});


if (window.top !== window.self) {
  showStatus("当前页面在 iframe 中", true)();
} 
    
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
    // 添加内存监控函数（可选）
function checkMemoryUsage() {
    if (performance.memory) {
        const memory = performance.memory;
        const usedMB = Math.round(memory.usedJSHeapSize / 1024 / 1024);
        const limitMB = Math.round(memory.jsHeapSizeLimit / 1024 / 1024);
        const usagePercent = (memory.usedJSHeapSize / memory.jsHeapSizeLimit * 100).toFixed(1);
        
        console.log(`内存使用: ${usedMB}MB / ${limitMB}MB (${usagePercent}%)`);
        
        // 如果内存使用超过80%就警告
        if (memory.usedJSHeapSize / memory.jsHeapSizeLimit > 0.8) {
            console.warn('To much mem used, need reflesh the page manually');
            return true;
        }
    }
    return false;
}
function cleanupOCR() {
    // 清理所有临时Canvas
    const tempCanvases = document.querySelectorAll('canvas.temp-ocr');
    tempCanvases.forEach(canvas => canvas.remove());
    
    // 清理可能存在的全局变量
    if (typeof lastImageData !== 'undefined') {
        lastImageData = null;
    }
    
    // 强制垃圾回收
    if (window.gc) {
        window.gc();
    }
    
    // 记录内存使用情况（开发时有用）
    if (performance.memory && console.log) {
        const memoryMB = Math.round(performance.memory.usedJSHeapSize / 1024 / 1024);
        console.log(`清理后内存使用: ${memoryMB}MB`);
    }
}
        
// 缓存频繁访问的DOM元素
const ocrElements = {
    fileInput: document.getElementById('fileInput'),
    preview: document.getElementById('ocrPreview'),
  preview2: document.getElementById('preview-image'),
    progress: document.getElementById('ocrProgress'),
    textInput: document.getElementById('textInput'),
    status: document.getElementById('status') || createStatusElement()
};

// 常量定义
const MAX_FILE_SIZE = 1 * 1024 * 1024; // 1MB
const MAX_IMAGE_DIMENSION = 1024;
const MAX_COMPRESSION_ATTEMPTS = 8;
const IMAGE_LOAD_TIMEOUT = 10000;
isOCRLoading = false; 

ocrElements.fileInput.addEventListener('change', async function(e) {
    const file = e.target.files[0];
    if (!file) return;
    
    // 提前验证文件类型
    if (!file.type.startsWith('image/') && !file.type.startsWith('text/') && !file.name.endsWith('.txt')) {
        showStatus('Not supported file type, only text or image supported', true);
        return;
    }
  
  if (!file.type.startsWith('image/')) {
            // 处理文本文件
      try{
        const text = await readTextFile(file);
        ocrElements.textInput.value = filterHighASCII(text);
        showStatus('Text loaded', false);
      }catch(error){
        showStatus('Fail to handle the Text content:'+ error.message, true);
      }
      return;
        }
  // 动态加载 ocrad.js
    if (typeof OCRAD === 'undefined'&& !isOCRLoading) {
    isOCRLoading = true; 
        ocrElements.progress.style.display = 'block';
        ocrElements.progress.textContent = 'Loading OCR engine...';      
        try {
            await loadScript('ocrad.js');     
            showStatus('OCR engine loaded', false);
      isOCRLoading = false;
        } catch (error) {
            showStatus('Failed to load OCR engine: ' + error.message, true);
            ocrElements.progress.style.display = 'none';
      isOCRLoading = false;
      return;
        }
    }
        processImageFile(file); // 如果已加载则直接处理
});
// 动态加载JS文件的函数
function loadScript(src) {
    return new Promise((resolve, reject) => {
        const script = document.createElement('script');
        script.src = src;
        script.onload = resolve;
        script.onerror = () => reject(new Error(`Script load error for ${src}`));
        document.head.appendChild(script);
    });
}

// 提取出来的图片处理逻辑
async function processImageFile(file){
    // 清理资源
    cleanupOCR();
   
    // 准备UI状态
    ocrElements.preview.style.display = 'none';
    ocrElements.progress.style.display = 'block';
    
    let resources = {
        imageUrl: null,
        canvas: null,
        img: null
    };
    
    try {      
            // 处理图片文件
            const processedFile = file.size > MAX_FILE_SIZE 
                ? await compressImageFile(file, MAX_FILE_SIZE) 
                : file;
            
            resources.imageUrl = URL.createObjectURL(processedFile);
            
            // 显示预览
            ocrElements.preview.style.display = 'block';
            ocrElements.preview.src = resources.imageUrl;
      ocrElements.preview2.src = resources.imageUrl;
            
            // 加载图片
            resources.img = await loadImage(resources.imageUrl);
            
            // 调整尺寸
            const { width, height } = calculateDimensions(resources.img);
            
            // 创建Canvas并绘制
            resources.canvas = createCanvas(width, height);
            const ctx = resources.canvas.getContext('2d');
            ctx.drawImage(resources.img, 0, 0, width, height);
            
            // 执行OCR识别
            const text = OCRAD(resources.canvas);
            ocrElements.textInput.value = filterHighASCII(text);
            showStatus('OCR done', false);      
    } catch (error) {
        handleFileError(error);
    } finally {
        cleanupResources(resources);
    }
}
        // Filter ASCII > 127 characters
        function filterHighASCII(text) {
            return text.replace(/[^\x00-\x7F]/g, '');
        }
// 辅助函数
async function loadImage(url) {
    return new Promise((resolve, reject) => {
        const img = new Image();
        img.src = url;
        
        const timer = setTimeout(() => {
            reject(new Error('Image loading overtime'));
        }, IMAGE_LOAD_TIMEOUT);
        
        img.onload = () => {
            clearTimeout(timer);
            resolve(img);
        };
        img.onerror = () => {
            clearTimeout(timer);
            reject(new Error('Image loading failed'));
        };
    });
}

function calculateDimensions(img) {
    let width = img.width;
    let height = img.height;
    
    if (width > MAX_IMAGE_DIMENSION || height > MAX_IMAGE_DIMENSION) {
        const ratio = Math.min(MAX_IMAGE_DIMENSION / width, MAX_IMAGE_DIMENSION / height);
        width = Math.floor(width * ratio);
        height = Math.floor(height * ratio);
    }
    
    return { width, height };
}

function createCanvas(width, height) {
    const canvas = document.createElement('canvas');
    canvas.width = width;
    canvas.height = height;
    canvas.className = 'temp-ocr';
    return canvas;
}

async function readTextFile(file) {
    return new Promise((resolve, reject) => {
        const reader = new FileReader();
        reader.onload = (e) => resolve(e.target.result);
        reader.onerror = () => reject(new Error('Fail to read'));
        reader.readAsText(file);
    });
}

function handleFileError(error) {
    console.error('File handling error:', error);
    
    let message = 'File handling error: ' + error.message;
    
    if (error.message.includes('Cannot enlarge memory')) {
        message = 'Image too big(>1MB)';
    } else if (error.message.includes('Image loading')) {
        message = 'Damaged image';
    }
    
    showStatus(message, true);
}

function cleanupResources(resources) {
    ocrElements.progress.style.display = 'none';
    
    if (resources.imageUrl) {
        URL.revokeObjectURL(resources.imageUrl);
    }
    
    if (resources.canvas) {
        resources.canvas.remove();
    }
    
    if (resources.img) {
        resources.img.src = '';
    }
    
    // 延迟垃圾回收
    if (window.gc) {
        setTimeout(window.gc, 100);
    }
}

// 文件压缩函数优化
async function compressImageFile(file, maxSize) {
    return new Promise((resolve) => {
        const canvas = document.createElement('canvas');
        const ctx = canvas.getContext('2d');
        const img = new Image();
        
        img.onload = function() {
            const { width, height, quality } = calculateCompressionParams(img, file.size, maxSize);
            
            canvas.width = width;
            canvas.height = height;
            ctx.drawImage(img, 0, 0, width, height);
            
            compressWithQuality(canvas, quality, maxSize, resolve);
        };
        
        img.src = URL.createObjectURL(file);
    });
}

function calculateCompressionParams(img, fileSize, maxSize) {
    const oversizeRatio = fileSize / maxSize;
    let quality = 0.8;
    let width = img.width;
    let height = img.height;
    
    // 根据超限比例调整质量和尺寸
    if (oversizeRatio > 4) {
        quality = 0.4;
        const scale = 0.6;
        width *= scale;
        height *= scale;
    } else if (oversizeRatio > 2) {
        quality = 0.6;
        const scale = 0.8;
        width *= scale;
        height *= scale;
    } else {
        quality = 0.7;
    }
    
    // 确保不超过最大尺寸
    if (width > MAX_IMAGE_DIMENSION || height > MAX_IMAGE_DIMENSION) {
        const ratio = Math.min(MAX_IMAGE_DIMENSION / width, MAX_IMAGE_DIMENSION / height);
        width *= ratio;
        height *= ratio;
    }
    
    return {
        width: Math.floor(width),
        height: Math.floor(height),
        quality
    };
}

function compressWithQuality(canvas, initialQuality, maxSize, resolve) {
    let quality = initialQuality;
    let attempts = 0;
    
    function tryCompress() {
        canvas.toBlob((blob) => {
            if (blob.size <= maxSize || quality <= 0.1 || attempts >= MAX_COMPRESSION_ATTEMPTS) {
                const finalFile = new File([blob], 'compressed.jpg', { type: 'image/jpeg' });
                canvas.remove();
                resolve(finalFile);
            } else {
                quality *= 0.8;
                attempts++;
                setTimeout(tryCompress, 0); // 给浏览器喘息时间
            }
        }, 'image/jpeg', quality);
    }
    
    tryCompress();
}

function createStatusElement() {
    const status = document.createElement('div');
    status.id = 'status';
    status.style.cssText = `
        position: fixed;
        top: 20px;
        right: 20px;
        padding: 10px 20px;
        border-radius: 4px;
        color: white;
        font-weight: bold;
        z-index: 1000;
        display: none;
    `;
    document.body.appendChild(status);
    
    // 添加样式
    const style = document.createElement('style');
    style.textContent = `
        .status.success { background-color: #4CAF50; }
        .status.error { background-color: #f44336; }
    `;
    document.head.appendChild(style);
    
    return status;
}





        async function sendText() {
            const mText = filterHighASCII(document.getElementById('textInput').value);
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
    let lastSendTime = 0;
    const MOUSE_UPDATE_INTERVAL = 20; // 50fps (1000/50=20ms)
    let lastPendingMove = null;
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
        let threasholdTime = 30;
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
            scrollY = event.touches.length > 0 ? event.touches[0].clientY : event.changedTouches[0].clientY;
            scrollX = event.touches.length > 0 ? event.touches[0].clientX : event.changedTouches[0].clientX;
            startTime = Date.now();
        }, { passive: false });
        mBt.addEventListener('touchend', (event) => {
            event.stopPropagation();
            event.preventDefault();
            if (middleClick) {
                //if{0,0} -> click
                var cx = event.touches.length > 0 ? event.touches[0].clientX : event.changedTouches[0].clientX;
                var cy = event.touches.length > 0 ? event.touches[0].clientY : event.changedTouches[0].clientY;
                sendMouse('Scroll',{x:Math.abs(scrollX - cx)>threashold?cx-scrollX:0,y:Math.abs(scrollY - cy)>threashold?cy-scrollY:0});
            }
            middleClick = false;
        }, { passive: false }); 
        mBt.addEventListener('touchcancel', (event) => {
            event.stopPropagation();
            event.preventDefault();
            if (middleClick) {
                var cx = event.touches.length > 0 ? event.touches[0].clientX : event.changedTouches[0].clientX;
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
        }, { passive: false });
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
                //console.log("action= " + mouseAction);
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

                    //console.log("pX= " + data.x);
                    //console.log("pY= " + data.y);
               if (Date.now() - startTime < threasholdTime) {
                             return;
                         }
             startTime = Date.now();
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
               if (Date.now() - startTime < threasholdTime) {
                             return;
                         }
             startTime = Date.now();
                    //console.log("pX= " + data.x);
                    //console.log("pY= " + data.y);
                    data.x = mTpw - data.x;
                    data.y = mTph - data.y;
                    response = await fetch(`/type?pAction=${mouseActionToKeyCode('Touch')}&pX=${encodeURIComponent(data.x)}&pY=${encodeURIComponent(data.y)}`, { method: 'GET' });
                } else {
                    //console.log("pA= " + data.press);
                    response = await fetch(`/type?pAction=${mouseActionToKeyCode(mouseAction)}&pA=${data.press}`, { method: 'GET' });
                }
                if (!response.ok) {
                    showStatus('Failed to send mouse actions', true);
                }
            } catch (error) {
                showStatus('Error: ' + error.message + '\n Failed to send mouse', true);
            }
        }
        document.addEventListener('touchmove', function(e) {
            if (e.target.tagName == 'CANVAS') {
        e.preventDefault();
      }          
        }, { passive: false });
        document.body.addEventListener('touchmove', function (e) {
      if (e.target.tagName == 'CANVAS') {
        e.preventDefault();
      }     
        }, { passive: false });
        window.addEventListener('touchmove', function (e) {
            if (window.scrollY === 0) {
        if (e.target.tagName == 'CANVAS') {
          e.preventDefault();
        }
      }
        }, { passive: false });

    </script>
</body>
</html>
)rawliteral";
