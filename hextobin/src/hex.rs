pub struct HexFile {
    lines: Vec<HexLine>
}

impl HexFile {
    fn parse_byte(char_iterator: &mut std::str::Chars) -> u8 {
        let mut byte = String::new();
        byte.push(char_iterator.next().unwrap());
        byte.push(char_iterator.next().unwrap());
        return u8::from_str_radix(byte.as_str(), 16).unwrap();
    }

    pub fn from(content: String) -> HexFile {
        let mut hex_lines = Vec::new();
        let lines = content.split("\n");

        for line in lines {
            if line == "" {
              continue;
            }

            let mut chars = line.chars();

            &chars.next(); // ignore initial colon

            let bc = HexFile::parse_byte(&mut chars);
            let addr = (HexFile::parse_byte(&mut chars) as u16) << 8 | (HexFile::parse_byte(&mut chars) as u16);
            let rectype = HexFile::parse_byte(&mut chars);

            let mut data: Vec<u8> = Vec::new();
            for _ in 0..bc {
                data.push(HexFile::parse_byte(&mut chars));
            }

            let checksum = HexFile::parse_byte(&mut chars);

            hex_lines.push(HexLine {
                bytes_count: bc,
                address: addr,
                record_type: rectype,
                data: data,
                checksum: checksum
            })
        }

        HexFile {
            lines: hex_lines
        }
    }

    pub fn to_binary(&self) -> Vec<u8> {
        let mut out = Vec::new();
        for line in &self.lines {
            out.append(&mut line.data.clone());
        }
        out
    }
}

struct HexLine {
    bytes_count: u8,
    address: u16,
    record_type: u8,
    data: Vec<u8>,
    checksum: u8
}