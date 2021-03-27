open System
open System.IO
open System.IO.Compression
open System.Text
open ICSharpCode.SharpZipLib.Zip.Compression.Streams

let decompress_raw_deflate path =
    use fs = File.OpenRead(path)
    use decompressionStream = new DeflateStream(fs, CompressionMode.Decompress)
    use memStream = new MemoryStream()
    decompressionStream.CopyTo(memStream)
    printfn "%A" memStream

let decompress path =
    use fs = File.OpenRead(path)
    let inflater = new InflaterInputStream(fs)
    let mutable bytes = Array.zeroCreate 1024
    let len = inflater.Read(bytes, 0, bytes.Length)
    inflater.Flush()
    printfn "%A" bytes
    printfn "%A" len
    let output = Encoding.UTF8.GetString(bytes, 0, len)
    printfn "%A" output

[<EntryPoint>]
let main argv =
    match Array.tryHead argv with
        | Some path -> decompress path
        | None -> printfn "usage: gitfs <path to zlib compressed file"
    0
