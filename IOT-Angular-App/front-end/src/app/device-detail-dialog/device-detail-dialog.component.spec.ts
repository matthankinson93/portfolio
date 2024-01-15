import { ComponentFixture, TestBed } from '@angular/core/testing';

import { DeviceDetailDialogComponent } from './device-detail-dialog.component';

describe('DeviceDetailDialogComponent', () => {
  let component: DeviceDetailDialogComponent;
  let fixture: ComponentFixture<DeviceDetailDialogComponent>;

  beforeEach(() => {
    TestBed.configureTestingModule({
      declarations: [DeviceDetailDialogComponent]
    });
    fixture = TestBed.createComponent(DeviceDetailDialogComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
