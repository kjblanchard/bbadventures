terraform {
  backend "s3" {
    bucket = "supergoon-terraform-plans"
    key    = "bbAdventures"
    region = "us-east-2"
  }
}