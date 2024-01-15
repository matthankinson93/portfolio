import { Component, OnInit } from '@angular/core';
import { MatDialogRef } from '@angular/material/dialog';
import { UserService } from '../services/user.service';
import { User } from '../models/user.model';
import { Device } from '../models/device.model';
import { DeviceService } from '../services/device.service';

@Component({
  selector: 'app-device-create-dialog',
  templateUrl: './device-create-dialog.component.html',
  styleUrls: ['./device-create-dialog.component.css']
})
export class DeviceCreateDialogComponent implements OnInit {
  users: User[] = [];
  device = { 
    name: '',
    owner_id: ''
  };

  constructor(
    public dialogRef: MatDialogRef<DeviceCreateDialogComponent>,
    private userService: UserService,
    private deviceService: DeviceService
  ) {}

  ngOnInit(): void {
    this.userService.getAllUsers().subscribe({
      next: (data) => {
        this.users = data;
      },
      error: (err) => {
        console.error('Failed to get users', err);
      }
    });
  }

  onNoClick(): void {
    this.dialogRef.close();
  }

  createDevice(): void {
    this.deviceService.createDevice(this.device).subscribe({
      next: (response) => {
        this.dialogRef.close(response);
      },
      error: (error) => {
        console.error('Error creating device:', error);
      }
    });
  }
}
