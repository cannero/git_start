use bincode;
use flate2::read::ZlibDecoder;
use serde::{Deserialize, Serialize};
use std::fs::File;
use std::io::prelude::*;

type ShaArr = [u8; 20];

#[derive(Serialize, Deserialize, Debug)]
struct CacheHeader {
    signature: u32,
    version: u32,
    num_entries: u32,
    sha1: ShaArr,
}

fn print_cache_header(path: String) {
    let mut v = Vec::new();
    File::open(path).unwrap().read_to_end(&mut v).unwrap();
    let header: CacheHeader = bincode::deserialize(&v).unwrap();
    println!("{:?}", header);
}

fn print_decompress(path: String) {
    let f = File::open(path).expect("file does not exist");
    let mut d = ZlibDecoder::new(f);
    let mut s = String::new();
    d.read_to_string(&mut s).unwrap();
    println!("{:?}", s);
}

fn usage() {
    eprintln!("--header/--decompress path_to_file");
}

fn main() {
    let mut args = std::env::args();
    if args.len() != 3 {
        usage();
        return;
    }

    args.next();
    let opt = args.next().unwrap();
    let path = args.next().unwrap();
    match opt.as_ref() {
        "-h" | "--header" => print_cache_header(path),
        "-d" | "--decompress" => print_decompress(path),
        _ => usage(),
    }
}
