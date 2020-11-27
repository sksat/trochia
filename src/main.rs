mod coordinate;
mod types;

use coordinate::local;
use coordinate::Local;
use types::Float;

fn main() {
    let _hoge: Float = 0.0;
    let _ned = local::NED::new(1.0, 2.0, 3.0);
    println!("Hello, world!");
}
