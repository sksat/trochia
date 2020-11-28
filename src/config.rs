extern crate toml;

use std::fs;
use std::io::{BufReader, Read};

#[derive(Debug, Deserialize)]
pub struct Config {
    // common simulation info
    simulation: Simulation,

    // environment info
    launcher: Launcher,
    wind: Option<Wind>,

    // rocket info
    rocket: Rocket,
}

#[derive(Debug, Deserialize)]
struct Simulation {
    dt: Option<f64>,
    output: Option<SimulationOutput>,
}
#[derive(Debug, Deserialize)]
struct SimulationOutput {
    dt: Option<f64>,
    dir: Option<String>,
}

#[derive(Debug, Deserialize)]
struct Launcher {
    length: f64,
    elevation: Vec<f64>,
}

#[derive(Debug, Deserialize)]
struct Wind {
    model: String,
    ground: WindGround,
}
#[derive(Debug, Deserialize)]
struct WindGround {
    dir: Vec<f64>,
    speed: Vec<f64>,
}

#[derive(Debug, Deserialize)]
struct Rocket {
    name: Option<String>,
    stage: Vec<RocketStage>,
}
#[derive(Debug, Deserialize)]
struct RocketStage {
    engine: String,
    diameter: f64,
    length: f64,
    weight: f64,
    I0: Option<f64>,
    If: Option<f64>,
    lcg0: Option<f64>,
    lcgf: Option<f64>,
    lcgp: Option<f64>,
    lcp: Option<f64>,
    Cd: f64,
    Cna: f64,
    parachute: Option<Parachute>,
}
#[derive(Debug, Deserialize)]
struct Parachute {
    condition: String,
    speed: f64,
    delay: Option<f64>,
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn sim() {
        let toml_str = r#"
        [simulation]
        dt = 0.01
        output.dt = 0.01
        output.dir = "output"

        [launcher]
        length = 5
        elevation = [ 90.0 ]

        [[rocket.stage]]
        type = "hybrid"
        engine = "hoge.eng"
        diameter = 0.0
        length = 0.0
        weight = 0.0
        Cd = 0.0
        Cna = 0.0
        "#;

        let cfg: Config = toml::from_str(toml_str).unwrap();
        println!("{:#?}", cfg);
        let sim = cfg.simulation;

        assert_eq!(sim.dt, Some(0.01));
    }

    #[test]
    fn example() {
        match fs::File::open("config-example.toml") {
            Ok(mut file) => {
                let mut content = String::new();
                file.read_to_string(&mut content);
                let cfg: Config = toml::from_str(&content).unwrap();
                println!("{:#?}", cfg);
            }
            Err(err) => {
                println!("error: {}", err);
            }
        }
    }
}
