﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Runtime.InteropServices;
using System.Windows.Forms;

namespace ModelViewer
{
    static class NativeMethods
    {
#if DEBUG
        const string EngineDLLName = "DLLExporter_Debug.dll";
#else
        const string EngineDLLName = "DLLExporter_Release.dll";
#endif
        
        [DllImport(EngineDLLName, CallingConvention = CallingConvention.Cdecl)]
        public extern static unsafe void StartEngine(IntPtr aHwnd);

        [DllImport(EngineDLLName, CallingConvention = CallingConvention.Cdecl)]
        public extern static unsafe void SetupWindow(Int32 aWidth, Int32 aHeight);

        [DllImport(EngineDLLName, CallingConvention = CallingConvention.Cdecl)]
        public extern static unsafe void Render();

        [DllImport(EngineDLLName, CallingConvention = CallingConvention.Cdecl)]
        public extern static unsafe void Update();

        [DllImport(EngineDLLName, CallingConvention = CallingConvention.Cdecl)]
        public extern static unsafe void LoadModel(string aModelFile, string aEffectFile);

        [DllImport(EngineDLLName, CallingConvention = CallingConvention.Cdecl)]
        public extern static unsafe void SetEffect(string aEffectFile);

        [DllImport(EngineDLLName, CallingConvention = CallingConvention.Cdecl)]
        public extern static unsafe void SetClearColor(float aRChannel, float aGChannel, float aBChannel, float aAChannel);

        [DllImport(EngineDLLName, CallingConvention = CallingConvention.Cdecl)]
        public extern static unsafe void DirectionaLightRotateX(float aXValue);

        [DllImport(EngineDLLName, CallingConvention = CallingConvention.Cdecl)]
        public extern static unsafe void DirectionaLightRotateY(float aYValue);

        [DllImport(EngineDLLName, CallingConvention = CallingConvention.Cdecl)]
        public extern static unsafe void DirectionaLightRotateZ(float aZValue);
    }
}
