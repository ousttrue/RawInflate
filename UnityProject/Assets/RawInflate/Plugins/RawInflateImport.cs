using System;
using System.Linq;
using System.IO;
using System.Runtime.InteropServices;
using UnityEngine;
using System.Collections.Generic;
#if UNITY_EDITOR
using UnityEditor;
#endif


namespace RawInflate
{
    static class RawInflateImport
    {
        const string DllName = "RawInflate";

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        public static extern int RawInflate(
            Byte[] dst, int dst_offset, int dst_size, 
            Byte[] src, int src_offset, int src_size
            );

#if UNITY_EDITOR
        static IEnumerable<string> EnumerateFiles(string path)
        {
            if (Path.GetFileName(path).StartsWith(".git"))
            {
                yield break;
            }

            if (Directory.Exists(path))
            {
                foreach (var child in Directory.GetFileSystemEntries(path))
                {
                    foreach (var x in EnumerateFiles(child))
                    {
                        yield return x;
                    }
                }
            }
            else
            {
                if (Path.GetExtension(path).ToLower() != ".meta")
                {
                    yield return path.Replace("\\", "/");
                }
            }
        }

        [MenuItem("UnityPackage/Export RawInflate")]
        static void Execute()
        {
            var path = Path.GetFullPath(Application.dataPath + "/../RawInflate.unitypackage");
            var files = EnumerateFiles("Assets/RawInflate").ToArray();
            AssetDatabase.ExportPackage(files
                , path
                , ExportPackageOptions.Default);
            Debug.LogFormat("Exported: {0}", path);
        }
#endif
    }
}
