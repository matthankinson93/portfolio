import { Injectable } from '@angular/core';
import { HttpClient } from '@angular/common/http';
import { Observable } from 'rxjs';

@Injectable({
  providedIn: 'root'
})
export class LoginService {
  private loginUrl = 'http://localhost:3000/api/login';

  constructor(private http: HttpClient) { }

  loginUser(user: { email: string; password: string }): Observable<any> {
    return this.http.post<any>(this.loginUrl, user);
  }
}