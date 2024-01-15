import { Component } from '@angular/core';
import { LoginService } from '../services/login.service';
import { NgForm } from '@angular/forms';
import { Router } from '@angular/router';

@Component({
  selector: 'app-login',
  templateUrl: './login.component.html',
  styleUrls: ['./login.component.css']
})
export class LoginComponent {
  emailPattern = "^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+_[a-zA-Z0-9.-]+\.[a-zA-Z]{2,}$";
  user = {
    email: '',
    password: ''
  };

  constructor(
    private loginService: LoginService,
    private router: Router
    ) {}

    onSubmit(form: NgForm) {
      this.loginService.loginUser(this.user).subscribe(
        (response: any) => {
          localStorage.setItem('token', response.token);
          this.router.navigate(['/overview']);
        },
        (error) => {
          console.log('Login failed', error);
        }
      );
    }
};