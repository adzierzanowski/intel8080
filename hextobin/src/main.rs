extern crate argparse;

mod hex;

use argparse::Store;
use std::fs;
use std::io::Write;
use std::process;

use hex::HexFile;

fn main() {
    let mut input_file = String::new();
    let mut output_file = String::new();

    let mut ap = argparse::ArgumentParser::new();
    ap.set_description("Convert between hex and binary files.");
    ap.refer(&mut input_file).add_option(&["-f", "--file"], Store, "Input file");
    ap.refer(&mut output_file).add_option(&["-o", "--output"], Store, "Output file");
    ap.parse_args_or_exit();
    drop(ap);

    let input_content = fs::read_to_string(input_file).unwrap_or_else(|err| {
        println!("Error reading hex file: {}.", err);
        process::exit(1);
    });

    let hex = HexFile::from(input_content);

    let mut file = fs::File::create(output_file).unwrap_or_else(|err| {
        println!("Error creating output file: {}.", err);
        process::exit(1);
    });

    let binary_output = hex.to_binary();

    file.write(&binary_output[..]).unwrap_or_else(|err| {
        println!("Error writing to output file: {}.", err);
        process::exit(1);
    });
}
