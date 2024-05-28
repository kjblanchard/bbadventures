terraform {
  backend "s3" {
    bucket = "supergoon-terraform-plans"
    key    = "cSupergoonWorld"
    region = "us-east-2"
  }
}